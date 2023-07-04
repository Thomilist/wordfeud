#include "VirtualBoard.hpp"


namespace wf
{
    VirtualBoard::VirtualBoard(Settings* a_settings)
        : settings(a_settings)
    { }
    
    VirtualBoard::VirtualBoard(
        Settings* a_settings,
        VirtualBoard* a_board)
        : settings(a_settings)
    {
        setWithBoard(a_board);
    }
    
    VirtualBoard::~VirtualBoard()
    { }
    
    void VirtualBoard::setWithBoard(VirtualBoard* a_board)
    {
        board.clear();
        discardProposed();
        proposed_play_evaluated = false;
        std::vector<VirtualTile> tile_collumn;
        
        for (int column = 0; column < a_board->getGridDimensions().width(); ++column)
        {
            for (int row = 0; row < a_board->getGridDimensions().height(); ++row)
            {
                tile_collumn.push_back(VirtualTile(a_board->getTileAtPosition(column, row)));
                tile_collumn.back().setGridPosition(column, row);
            }

            board.push_back(tile_collumn);
            tile_collumn.clear();
        }

        grid_dimensions = a_board->getGridDimensions();
        assignNeighbours();

        return;
    }
    
    void VirtualBoard::importProposedLetters(std::vector<VirtualTile*> a_tiles)
    {
        discardProposed();

        QPoint position;
        Letter* letter;
        VirtualTile* destination_tile;
        
        for (auto origin_tile : a_tiles)
        {
            position = origin_tile->getGridPosition();
            letter = origin_tile->getLetter();
            destination_tile = getTileAtPosition(position.x(), position.y());

            destination_tile->placeLetter(letter);
            proposeLetter(destination_tile);
        }

        return;
    }
    
    std::vector<VirtualTile*> VirtualBoard::getProposedLetters()
    {
        return proposed_letters;
    }
    
    VirtualTile* VirtualBoard::getTileAtPosition(int a_column, int a_row)
    {
        if (    a_column < 0
            ||  a_row < 0
            ||  a_column >= grid_dimensions.width()
            ||  a_row >= grid_dimensions.height())
        {
            return nullptr;
        }
        
        return &board[a_column][a_row];
    }
    
    VirtualTile* VirtualBoard::getTileAtPosition(QPoint a_position)
    {
        return getTileAtPosition(a_position.x(), a_position.y());
    }
    
    QSize VirtualBoard::getGridDimensions() const
    {
        return grid_dimensions;
    }
    
    int VirtualBoard::getGridDimensionInDirection(Direction a_direction)
    {
        int size = 0;
        
        switch (a_direction)
        {
            case Direction::Horisontal:
            {
                size = getGridDimensions().width();
                break;
            }
            case Direction::Vertical:
            {
                size = getGridDimensions().height();
                break;
            }
        }

        return size;
    }
    
    int VirtualBoard::getTileCount() const
    {
        return getGridDimensions().width() * getGridDimensions().height();
    }
    
    int VirtualBoard::getLetterCount()
    {
        int count = 0;
        Letter* letter;
        
        for (int column = 0; column < getGridDimensions().width(); ++column)
        {
            for (int row = 0; row < getGridDimensions().height(); ++row)
            {
                letter = getTileAtPosition(column, row)->getLetter();

                if (letter != nullptr)
                {
                    ++count;
                }
            }
        }

        return count;
    }
    
    void VirtualBoard::evaluateProposedPlay(bool a_force, bool a_skip_placement_check, bool a_exit_early)
    {
        if (a_force)
        {
            proposed_play_evaluated = false;
        }

        if (a_skip_placement_check)
        {
            proposed_placement_valid = true;
        }

        if (proposed_play_evaluated)
        {
            return;
        }

        proposed_play_points = 0;

        if (!a_skip_placement_check && !isPlacementValid())
        {
            proposed_words_valid = false;
            proposed_play_evaluated = true;
            return;
        }

        findProposedWords(a_exit_early);
        
        if (proposed_words_valid)
        {
            findInvalidProposedWords();
            calculateProposedPoints();
        }

        proposed_play_evaluated = true;

        return;
    }
    
    bool VirtualBoard::isPlacementValid()
    {
        if (proposed_play_evaluated)
        {
            return proposed_placement_valid;
        }
        
        updateProposedLines();
        proposed_placement_valid = true;

        for (const auto letter : proposed_letters)
        {
            checked_tiles.clear();
            
            if (!isPlacementConnectedToStart(letter))
            {
                return proposed_placement_valid = false;
            }
        }

        if (!isPlacementLinear())
        {
            return proposed_placement_valid = false;
        }

        if (!isPlacementContiguous())
        {
            return proposed_placement_valid = false;
        }

        return proposed_placement_valid;
    }
    
    bool VirtualBoard::areProposedWordsValid()
    {
        if (!proposed_play_evaluated)
        {
            evaluateProposedPlay();
        }

        return proposed_words_valid;
    }
    
    bool VirtualBoard::isProposedPlayValid()
    {
        if (!proposed_play_evaluated)
        {
            evaluateProposedPlay();
        }

        return proposed_placement_valid && proposed_words_valid;
    }
    
    int VirtualBoard::getProposedLetterCount()
    {
        return proposed_letters.size();
    }
    
    std::vector<Word> VirtualBoard::getProposedWords()
    {
        return proposed_words;
    }
    
    std::vector<Word> VirtualBoard::getInvalidProposedWords()
    {
        return invalid_words;
    }
    
    int VirtualBoard::getProposedPlayPoints(bool a_skip_validation)
    {
        if (!proposed_play_evaluated && !a_skip_validation)
        {
            evaluateProposedPlay();
        }

        return proposed_play_points;
    }
    
    Word VirtualBoard::getMostRelevantWord()
    {
        if (!proposed_play_evaluated)
        {
            evaluateProposedPlay();
        }
        
        int highest_match_count = 0;
        Word most_relevant_word;

        for (const auto& word : proposed_words)
        {
            int match_count = word.containsHowManyOf(proposed_letters);

            if (match_count > highest_match_count)
            {
                highest_match_count = match_count;
                most_relevant_word = word;
            }
        }

        return most_relevant_word;
    }
    
    void VirtualBoard::lockProposedPlay()
    {
        for (const auto tile : proposed_letters)
        {
            tile->getModifier()->setUsed(true);
        }

        lockRecentlyLockedLetters();
        lockProposedLetters();
        proposed_play_evaluated = false;

        return;
    }
    
    void VirtualBoard::lockProposedLetters()
    {
        for (auto tile : proposed_letters)
        {
            Letter* letter = tile->getLetter();
            letter->setStatus(LetterStatus::LockedRecently);
            locked_letters.push_back(letter);

            Modifier* modifier = tile->getModifier();
            modifier->setUsed(true);
        }

        proposed_letters.clear();

        return;
    }
    
    void VirtualBoard::lockRecentlyLockedLetters()
    {
        for (auto letter : locked_letters)
        {
            if (letter->getStatus() == LetterStatus::LockedRecently)
            {
                letter->setStatus(LetterStatus::Locked);
            }
        }

        return;
    }
    
    void VirtualBoard::proposeLetter(VirtualTile* a_tile)
    {
        proposed_letters.push_back(a_tile);
        proposed_play_evaluated = false;
        return;
    }
    
    void VirtualBoard::proposeLetter(QPoint a_position)
    {
        VirtualTile* tile = getTileAtPosition(a_position);
        proposeLetter(tile);
        return;
    }
    
    void VirtualBoard::unproposeLetter(VirtualTile* a_tile)
    {
        proposed_letters.erase(std::remove(proposed_letters.begin(), proposed_letters.end(), a_tile), proposed_letters.end());
        proposed_play_evaluated = false;
        return;
    }
    
    void VirtualBoard::unproposeLetter(QPoint a_position)
    {
        VirtualTile* tile = getTileAtPosition(a_position);
        unproposeLetter(tile);
        return;
    }
    
    void VirtualBoard::placeLetter(QPoint a_position, Letter* a_letter)
    {
        VirtualTile* tile = getTileAtPosition(a_position);
        tile->placeLetter(a_letter);
        return;
    }
    
    Letter* VirtualBoard::removeLetter(QPoint a_position)
    {
        VirtualTile* tile = getTileAtPosition(a_position);
        return tile->removeLetter();
    }
    
    void VirtualBoard::assignNeighbours()
    {
        VirtualTile* tile;
        
        for (int column = 0; column < grid_dimensions.width(); ++column)
        {
            for (int row = 0; row < grid_dimensions.height(); ++row)
            {
                tile = getTileAtPosition(column, row);

                tile->setNeighbour(getTileAtPosition(column, row - 1), TileNeighbour::Top);
                tile->setNeighbour(getTileAtPosition(column + 1, row), TileNeighbour::Right);
                tile->setNeighbour(getTileAtPosition(column, row + 1), TileNeighbour::Bottom);
                tile->setNeighbour(getTileAtPosition(column - 1, row), TileNeighbour::Left);
            }
        }

        return;
    }
    
    [[nodiscard]] std::vector<Letter*> VirtualBoard::clearProposed()
    {
        std::vector<Letter*> letters;
        letters.reserve(proposed_letters.size());

        for (auto tile : proposed_letters)
        {
            if (tile->getLetter() != nullptr)
            {
                letters.push_back(tile->VirtualTile::removeLetter());
            }
        }

        proposed_letters.clear();
        proposed_play_evaluated = false;
        return letters;
    }
    
    void VirtualBoard::discardProposed()
    {
        proposed_letters.clear();
        proposed_play_evaluated = false;
        return;
    }
    
    void VirtualBoard::invalidateEvaluation()
    {
        proposed_play_evaluated = false;
        return;
    }
    
    bool VirtualBoard::isPlacementConnectedToStart(VirtualTile* a_tile)
    {
        checked_tiles.push_back(a_tile);
        
        if (a_tile->getModifier()->getType() == ModifierType::Start)
        {
            return true;
        }

        std::array<VirtualTile*, 4> neighbours = a_tile->getNeighbours();
        bool start_found = false;

        for (const auto neighbour : neighbours)
        {
            if (neighbour == nullptr)
            {
                continue;
            }
            
            if (std::find(checked_tiles.begin(), checked_tiles.end(), neighbour) != checked_tiles.end())
            {
                continue;
            }
            
            if (neighbour->getLetter() != nullptr)
            {
                start_found = start_found || isPlacementConnectedToStart(neighbour);
            }
        }

        return start_found;
    }
    
    bool VirtualBoard::isPlacementLinear()
    {
        return isPlacementInOneCollumn() || isPlacementInOneRow();
    }
    
    bool VirtualBoard::isPlacementInOneCollumn()
    {
        return std::adjacent_find(proposed_columns.begin(), proposed_columns.end(), std::not_equal_to<>()) == proposed_columns.end();
    }
    
    bool VirtualBoard::isPlacementInOneRow()
    {
        return std::adjacent_find(proposed_rows.begin(), proposed_rows.end(), std::not_equal_to<>()) == proposed_rows.end();
    }
    
    bool VirtualBoard::isPlacementContiguous()
    {
        if (proposed_columns.size() == 0 || proposed_rows.size() == 0)
        {
            return true;
        }
        
        // The contiguity check assumes the proposed letters are in a single line
        int minimum_column = *proposed_columns.begin();
        int maximum_column = *proposed_columns.rbegin();
        int minimum_row = *proposed_rows.begin();
        int maximum_row = *proposed_rows.rbegin();

        VirtualTile* tile;

        for (int column = minimum_column; column <= maximum_column; ++column)
        {
            for (int row = minimum_row; row <= maximum_row; ++row)
            {
                tile = getTileAtPosition(column, row);

                if (tile->getLetter() == nullptr)
                {
                    return false;
                }
            }
        }

        return true;
    }
    
    void VirtualBoard::updateProposedLines()
    {
        proposed_columns.clear();
        proposed_rows.clear();
        
        for (const auto tile : proposed_letters)
        {
            proposed_columns.insert(tile->getGridPosition().x());
        }
        
        for (const auto tile : proposed_letters)
        {
            proposed_rows.insert(tile->getGridPosition().y());
        }

        return;
    }
    
    void VirtualBoard::findProposedWords(bool a_exit_early)
    {
        proposed_words.clear();
        proposed_words_valid = true;
        
        if (proposed_letters.empty())
        {
            return;
        }
        // Single-letter words are allowed only as an opener
        else if (   proposed_letters.size() == 1
                &&  proposed_letters[0]->getModifier()->getType() == ModifierType::Start)
        {
            Word word;
            word.appendLetter(proposed_letters[0]);
            proposed_words.push_back(word);
            return;
        }
        
        std::set<int> collumns;
        std::set<int> rows;

        for (const auto& tile : proposed_letters)
        {
            QPoint grid_position = tile->getGridPosition();

            collumns.insert(grid_position.x());
            rows.insert(grid_position.y());
        }

        // Valid placements are always in a line
        // Check along the line first, since this always needs to be checked, allowing for a potential early exit
        // One-letter words don't count unless it's the first word on the board and just one letter is placed,
        // so checking across first would be a waste of time if the word along the line causes an early exit
        Direction first_direction = collumns.size() > rows.size() ? Direction::Horisontal : Direction::Vertical;
        Direction second_direction = first_direction == Direction::Horisontal ? Direction::Vertical : Direction::Horisontal;

        Word word;

        for (auto direction : {first_direction, second_direction})
        {
            for (auto& tile : proposed_letters)
            {
                word = findWordWithLetter(tile, direction);

                if (word.getLength() > 1)
                {
                    if (a_exit_early)
                    {
                        if (!settings->getCurrentLanguage()->isInWordList(word.getWordAsText()))
                        {
                            proposed_words_valid = false;
                            return;
                        }
                    }
                    
                    proposed_words.push_back(word);
                }

                if (direction == first_direction)
                {
                    break;
                }
            }
        }

        return;
    }
    
    Word VirtualBoard::findWordWithLetter(VirtualTile* a_tile, Direction a_direction)
    {
        QPoint starting_position = a_tile->getGridPosition();
        std::deque<VirtualTile*> tiles;
        tiles.push_back(a_tile);
        int collumn_shift = 0;
        int row_shift = 0;

        switch (a_direction)
        {
            case Direction::Horisontal:
            {
                collumn_shift = 1;
                break;
            }
            case Direction::Vertical:
            {
                row_shift = 1;
                break;
            }
        }

        VirtualTile* next_tile;
        int column = starting_position.x();
        int row = starting_position.y();

        // Search left or up
        while (true)
        {
            column -= collumn_shift;
            row -= row_shift;
            next_tile = getTileAtPosition(column, row);

            if (next_tile == nullptr)
            {
                break;
            }

            if (next_tile->getLetter() == nullptr)
            {
                break;
            }

            tiles.push_front(next_tile);
        }

        column = starting_position.x();
        row = starting_position.y();

        // Search right or down
        while (true)
        {
            column += collumn_shift;
            row += row_shift;
            next_tile = getTileAtPosition(column, row);

            if (next_tile == nullptr)
            {
                break;
            }

            if (next_tile->getLetter() == nullptr)
            {
                break;
            }

            tiles.push_back(next_tile);
        }

        // Create word
        Word word;

        for (auto tile : tiles)
        {
            word.appendLetter(tile);
        }

        return word;
    }
    
    void VirtualBoard::findInvalidProposedWords()
    {
        invalid_words.clear();
        proposed_words_valid = true;

        for (auto word : proposed_words)
        {
            if (!settings->getCurrentLanguage()->isInWordList(word.getWordAsText()))
            {
                invalid_words.push_back(word);
                proposed_words_valid = false;
            }
        }

        return;
    }
    
    void VirtualBoard::calculateProposedPoints()
    {
        proposed_play_points = 0;

        for (const auto& word : proposed_words)
        {
            proposed_play_points += word.calculatePoints();
        }

        std::size_t hand_capacity = settings->getHandDimensions().width() * settings->getHandDimensions().height();
        
        if (proposed_letters.size() == hand_capacity)
        {
            proposed_play_points += 40;
        }

        return;
    }
}