#include "PlayerAI.hpp"


namespace wf
{
    PlayerAI::PlayerAI(
        QString a_display_name,
        QColor a_color,
        Settings* a_settings,
        RenderedTile* a_selection,
        RenderedBoard* a_board)
        : Player(
            a_display_name,
            PlayerType::AI,
            a_color,
            a_settings,
            a_selection)
        , live_board(a_board)
        , sandbox_board(
            a_settings,
            a_board)
        , best_play(
            a_settings,
            a_board)
    {
        initialiseBoardEvaluation(live_board);
    }
    
    PlayerAI::~PlayerAI()
    { }
    
    void PlayerAI::playIfTurn()
    {
        if (hasTurn())
        {
            executeTurn();
        }

        return;
    }
    
    void PlayerAI::cancelTurn()
    {
        cancelled = true;
        return;
    }
    
    void PlayerAI::executeTurn()
    {
        startOfTurnSetup();
        findBestPlay();

        if (cancelled)
        {
            return;
        }

        executeBestPlay();
        endTurn();
        
        return;
    }
    
    void PlayerAI::endTurn()
    {
        if (best_play.getProposedPlayPoints() > 0)
        {
            emit playComplete();
        }

        return;
    }
    
    void PlayerAI::startOfTurnSetup()
    {
        sandbox_board.setWithBoard(live_board);
        best_play.setWithBoard(live_board);
        fetchAvailableLetters();
        updateRelevantLines();
        evaluateBoard(live_board);

        return;
    }
    
    void PlayerAI::fetchAvailableLetters()
    {
        available_letters.clear();
        available_letter_count = 0;
        Letter* letter;

        for (int row = 0; row < getHand()->getGridDimensions().height(); ++row)
        {
            for (int collumn = 0; collumn < getHand()->getGridDimensions().width(); ++collumn)
            {
                letter = getHand()->getTileAtPosition(collumn, row)->getLetter();

                if (letter != nullptr)
                {
                    available_letters.push_back(letter);
                    ++available_letter_count;
                }
            }
        }

        return;
    }
    
    void PlayerAI::findBestPlay()
    {
        for (Direction direction : {Direction::Horisontal, Direction::Vertical})
        {
            for (   int index = 0;
                    index < live_board->getGridDimensionInDirection(
                        // The number of horisontal lines is the grid size in the vertical direction and vice versa
                        direction == Direction::Horisontal ? Direction::Vertical : Direction::Horisontal
                    );
                    ++index
                )
            {
                findPlayInLine(direction, index);
            }
        }

        return;
    }
    
    void PlayerAI::findPlayInLine(Direction a_direction, int a_index)
    {
        switch (a_direction)
        {
            case Direction::Horisontal:
            {
                if (relevant_rows.contains(a_index))
                {
                    findPlayInHorisontalLine(a_index);
                }

                break;
            }
            case Direction::Vertical:
            {
                if (relevant_collumns.contains(a_index))
                {
                    findPlayInVerticalLine(a_index);
                }
                
                break;
            }
        }

        return;
    }
    
    void PlayerAI::findPlayInHorisontalLine(int a_row)
    {
        for (int collumn = 0; collumn < live_board->getGridDimensionInDirection(Direction::Horisontal); ++collumn)
        {
            findPlayRecursively(collumn, a_row, Direction::Horisontal);
        }

        return;
    }
    
    void PlayerAI::findPlayInVerticalLine(int a_collumn)
    {
        for (int row = 0; row < live_board->getGridDimensionInDirection(Direction::Vertical); ++row)
        {
            findPlayRecursively(a_collumn, row, Direction::Vertical);
        }

        return;
    }
    
    // For horisontal, fixed index = row, variable index = collumn
    // For vertical, fixed index = collumn, variable index = row
    void PlayerAI::findPlayRecursively(int a_collumn, int a_row, Direction a_direction)
    {
        if (cancelled)
        {
            return;
        }
        
        if (indexOutOfBounds(a_collumn, a_row))
        {
            return;
        }

        VirtualTile* tile = sandbox_board.getTileAtPosition(a_collumn, a_row);
        bool tile_available = tile->getLetter() == nullptr;

        if (tile_available)
        {
            for (auto& letter : available_letters)
            {
                if (letter == nullptr)
                {
                    continue;
                }

                if (letter->getType() == LetterType::Wildcard)
                {
                    auto letter_list = settings->getLanguage()->getLetterList();

                    for (auto letter_option : letter_list)
                    {
                        letter->setWildcardText(letter_option.letter);
                        tryLetterAndRecurse(letter, tile, a_collumn, a_row, a_direction);
                    }
                }
                else
                {
                    tryLetterAndRecurse(letter, tile, a_collumn, a_row, a_direction);
                }
            }
        }
        else
        {
            recurse(a_collumn, a_row, a_direction);
        }

        return;
    }
    
    void PlayerAI::tryLetterAndRecurse(Letter*& a_letter, VirtualTile* a_tile, int a_collumn, int a_row, Direction a_direction)
    {
        a_tile->placeLetter(a_letter);
        a_letter = nullptr;
        sandbox_board.proposeLetter(a_tile);
        --available_letter_count;
        touch_count += board_evaluation[a_collumn][a_row];

        if (touch_count > 0)
        {
            updateBestPlay();
        }

        if (available_letter_count > 0)
        {
            recurse(a_collumn, a_row, a_direction);
        }
        
        a_letter = a_tile->removeLetter();
        sandbox_board.unproposeLetter(a_tile);
        ++available_letter_count;
        touch_count -= board_evaluation[a_collumn][a_row];

        return;
    }
    
    void PlayerAI::recurse(int a_collumn, int a_row, Direction a_direction)
    {
        switch (a_direction)
        {
            case Direction::Horisontal:
            {
                findPlayRecursively(a_collumn + 1, a_row, a_direction);
                break;
            }
            case Direction::Vertical:
            {
                findPlayRecursively(a_collumn, a_row + 1, a_direction);
                break;
            }
        }

        return;
    }
    
    bool PlayerAI::indexOutOfBounds(int a_collumn, int a_row)
    {
        return a_collumn >= sandbox_board.getGridDimensionInDirection(Direction::Horisontal) || a_row >= sandbox_board.getGridDimensionInDirection(Direction::Vertical);
    }
    
    void PlayerAI::updateBestPlay()
    {
        best_play.evaluateProposedPlay();
        sandbox_board.evaluateProposedPlay();
        
        if (!sandbox_board.isProposedPlayValid())
        {
            return;
        }

        if (sandbox_board.getProposedPlayPoints() > best_play.getProposedPlayPoints())
        {
            best_play.setWithBoard(&sandbox_board);
            best_play.importProposedLetters(sandbox_board.getProposedLetters());
            setBestPlayWildcardLetters(best_play.getProposedLetters());
        }

        return;
    }
    
    void PlayerAI::executeBestPlay()
    {
        QPoint position;
        Letter* letter;
        VirtualTile* live_tile;
        int index = 0;
        
        for (auto local_tile : best_play.getProposedLetters())
        {
            QThread::msleep(100);
            
            position = local_tile->getGridPosition();
            letter = local_tile->getLetter();
            live_tile = live_board->getTileAtPosition(position.x(), position.y());

            // Wildcard letters must have their letter contents restored
            if (letter->getType() == LetterType::Wildcard)
            {
                letter->setWildcardText(best_play_wildcard_letters.at(index));
            }

            // The live board holds rendered tiles, which automatically propose a letter when placed
            live_tile->placeLetter(letter);

            // Remove letter from hand
            bool letter_removed = false;
            VirtualTile* tile;

            for (int row = 0; row < getHand()->getGridDimensions().height() && !letter_removed; ++row)
            {
                for (int collumn = 0; collumn < getHand()->getGridDimensions().width() && !letter_removed; ++collumn)
                {
                    tile = getHand()->getTileAtPosition(collumn, row);
                    
                    if (tile->getLetter() == letter)
                    {
                        tile->removeLetter();
                        letter_removed = true;
                    }
                }
            }

            emit letterPlaced();
            ++index;
        }
    }
    
    void PlayerAI::setBestPlayWildcardLetters(std::vector<VirtualTile*> a_tiles)
    {
        best_play_wildcard_letters.clear();
        QChar letter;

        for (auto tile : a_tiles)
        {
            if (tile->getLetter()->getType() == LetterType::Wildcard)
            {
                letter = tile->getLetter()->getWildcardText();
            }
            else
            {
                letter = QChar{};
            }

            best_play_wildcard_letters.push_back(letter);
        }

        return;
    }
    
    void PlayerAI::updateRelevantLines()
    {
        relevant_collumns.clear();
        relevant_rows.clear();

        VirtualTile* tile;

        for (int collumn = 0; collumn < live_board->getGridDimensions().width(); ++collumn)
        {
            for (int row = 0; row < live_board->getGridDimensions().height(); ++row)
            {
                tile = live_board->getTileAtPosition(collumn, row);

                // A collumn or row is relevant if the start tile or a letter is on or adjacent to it
                if (tile->getLetter() != nullptr || tile->getModifier()->getType() == ModifierType::Start)
                {
                    // Add collumn before, at and after
                    if (collumn - 1 >= 0)
                    {
                        relevant_collumns.insert(collumn - 1);
                    }

                    relevant_collumns.insert(collumn);

                    if (collumn + 1 <= live_board->getGridDimensions().width())
                    {
                        relevant_collumns.insert(collumn + 1);
                    }

                    // Add row before, at and after
                    if (row - 1 >= 0)
                    {
                        relevant_rows.insert(row - 1);
                    }

                    relevant_rows.insert(row);

                    if (row + 1 <= live_board->getGridDimensions().height())
                    {
                        relevant_rows.insert(row + 1);
                    }
                }
            }
        }

        return;
    }
    
    void PlayerAI::initialiseBoardEvaluation(VirtualBoard* a_board)
    {
        board_evaluation.reserve(a_board->getGridDimensions().width());
        std::vector<int> collumn_evaluation;
        collumn_evaluation.reserve(a_board->getGridDimensions().height());

        for (int collumn = 0; collumn < a_board->getGridDimensions().width(); ++collumn)
        {
            collumn_evaluation.clear();
            
            for (int row = 0; row < a_board->getGridDimensions().height(); ++row)
            {
                collumn_evaluation.push_back(0);
            }

            board_evaluation.push_back(collumn_evaluation);
        }

        return;
    }
    
    void PlayerAI::evaluateBoard(VirtualBoard* a_board)
    {
        VirtualTile* tile;

        for (int collumn = 0; collumn < a_board->getGridDimensions().width(); ++collumn)
        {
            for (int row = 0; row < a_board->getGridDimensions().height(); ++row)
            {
                tile = a_board->getTileAtPosition(collumn, row);
                board_evaluation[collumn][row] = evaluateTile(tile);
            }
        }

        return;
    }
    
    int PlayerAI::evaluateTile(VirtualTile* a_tile)
    {
        if (a_tile->getLetter() != nullptr)
        {
            return 0;
        }

        if (a_tile->getModifier()->getType() == ModifierType::Start)
        {
            return 1;
        }
        
        auto neighbours = a_tile->getNeighbours();

        for (auto neighbour : neighbours)
        {
            if (neighbour == nullptr)
            {
                continue;
            }

            if (neighbour->getLetter() != nullptr)
            {
                return 1;
            }
        }

        return 0;
    }
}