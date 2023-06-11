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
        clearProposed();
        proposed_play_evaluated = false;
        std::vector<VirtualTile> tile_collumn;
        
        for (int collumn = 0; collumn < a_board->getGridDimensions().width(); ++collumn)
        {
            for (int row = 0; row < a_board->getGridDimensions().height(); ++row)
            {
                tile_collumn.push_back(VirtualTile(a_board->getTileAtPosition(collumn, row)));
                tile_collumn.back().setGridPosition(collumn, row);
            }

            board.push_back(tile_collumn);
            tile_collumn.clear();
        }

        assignNeighbours();
        grid_dimensions = a_board->getGridDimensions();

        return;
    }
    
    void VirtualBoard::importProposedLetters(std::vector<VirtualTile*> a_tiles)
    {
        clearProposed();

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
    
    VirtualTile* VirtualBoard::getTileAtPosition(int a_collumn, int a_row)
    {
        if (    a_collumn < 0
            ||  a_row < 0
            ||  a_collumn >= grid_dimensions.width()
            ||  a_row >= grid_dimensions.height())
        {
            return nullptr;
        }
        
        return &board[a_collumn][a_row];
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
        
        for (int collumn = 0; collumn < getGridDimensions().width(); ++collumn)
        {
            for (int row = 0; row < getGridDimensions().height(); ++row)
            {
                letter = getTileAtPosition(collumn, row)->getLetter();

                if (letter != nullptr)
                {
                    ++count;
                }
            }
        }

        return count;
    }
    
    void VirtualBoard::evaluateProposedPlay(bool a_force)
    {
        if (a_force)
        {
            proposed_play_evaluated = false;
        }

        if (proposed_play_evaluated)
        {
            return;
        }

        proposed_play_points = 0;
        
        if (!isPlacementValid())
        {
            proposed_words_valid = false;
            proposed_play_evaluated = true;
            return;
        }

        findProposedWords();
        findInvalidProposedWords();
        calculateProposedPoints();

        proposed_play_evaluated = true;

        return;
    }
    
    bool VirtualBoard::isPlacementValid()
    {
        if (proposed_play_evaluated)
        {
            return proposed_placement_valid;
        }
        
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
    
    std::vector<Word*> VirtualBoard::getInvalidProposedWords()
    {
        return invalid_words;
    }
    
    int VirtualBoard::getProposedPlayPoints()
    {
        if (!proposed_play_evaluated)
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
    
    void VirtualBoard::unproposeLetter(VirtualTile* a_tile)
    {
        proposed_letters.erase(std::remove(proposed_letters.begin(), proposed_letters.end(), a_tile), proposed_letters.end());
        proposed_play_evaluated = false;
        return;
    }
    
    void VirtualBoard::assignNeighbours()
    {
        for (int collumn = 0; collumn < grid_dimensions.width(); ++collumn)
        {
            for (int row = 0; row < grid_dimensions.height(); ++row)
            {
                VirtualTile* tile = getTileAtPosition(collumn, row);

                tile->setNeighbour(getTileAtPosition(collumn, row - 1), TileNeighbour::Top);
                tile->setNeighbour(getTileAtPosition(collumn + 1, row), TileNeighbour::Right);
                tile->setNeighbour(getTileAtPosition(collumn, row + 1), TileNeighbour::Bottom);
                tile->setNeighbour(getTileAtPosition(collumn - 1, row), TileNeighbour::Left);
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
        std::vector<int> collumns;
        
        for (const auto tile : proposed_letters)
        {
            collumns.push_back(tile->getGridPosition().x());
        }

        return std::adjacent_find(collumns.begin(), collumns.end(), std::not_equal_to<>()) == collumns.end();
    }
    
    bool VirtualBoard::isPlacementInOneRow()
    {
        std::vector<int> rows;
        
        for (const auto tile : proposed_letters)
        {
            rows.push_back(tile->getGridPosition().y());
        }

        return std::adjacent_find(rows.begin(), rows.end(), std::not_equal_to<>()) == rows.end();
    }
    
    void VirtualBoard::findProposedWords()
    {
        proposed_words.clear();
        
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

        std::unordered_set<int> collumns;
        std::unordered_set<int> rows;

        for (const auto tile : proposed_letters)
        {
            QPoint grid_position = tile->getGridPosition();

            collumns.insert(grid_position.x());
            rows.insert(grid_position.y());
        }

        for (const auto collumn : collumns)
        {
            findProposedWordInLine(collumn, settings->getBoardDimensions().height(), Direction::Vertical);
        }

        for (const auto row : rows)
        {
            findProposedWordInLine(row, settings->getBoardDimensions().width(), Direction::Horisontal);
        }

        return;
    }
    
    void VirtualBoard::findProposedWordInLine(int a_line, int a_max_index, Direction a_direction)
    {
        Word word;
            
        for (int index = 0; index < a_max_index; ++index)
        {
            VirtualTile* tile = nullptr;

            switch (a_direction)
            {
                case Direction::Horisontal:
                {
                    tile = getTileAtPosition(index, a_line);
                    break;
                }
                case Direction::Vertical:
                {
                    tile = getTileAtPosition(a_line, index);
                    break;
                }
            }

            if (tile->getLetter() == nullptr)
            {
                if (word.containsAnyOf(proposed_letters))
                {
                    if (word.getLength() > 1)
                    {
                        proposed_words.push_back(word);
                    }
                    
                    break;
                }
                else
                {
                    word.clear();
                }
            }
            else
            {
                word.appendLetter(tile);

                if (index == a_max_index - 1)
                {
                    if (    word.containsAnyOf(proposed_letters)
                        &&  word.getLength() > 1)
                    {
                        proposed_words.push_back(word);
                        break;
                    }
                }
            }
        }

        return;
    }
    
    void VirtualBoard::findInvalidProposedWords()
    {
        invalid_words.clear();
        proposed_words_valid = true;

        for (auto& word : proposed_words)
        {
            if (!settings->getLanguage()->isInWordList(word.getWordAsText().toLower()))
            {
                invalid_words.push_back(&word);
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