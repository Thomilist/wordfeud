#include "PlayerAIWorker.hpp"


namespace wf
{
    PlayerAIWorker::PlayerAIWorker(
        Settings* a_settings,
        VirtualBoard* a_live_board,
        VirtualBoard* a_hand,
        Direction a_direction,
        int a_line_index,
        std::set<int> a_relevant_rows,
        std::set<int> a_relevant_columns,
        std::vector<std::vector<int>> a_touch_evaluation)
        : settings(a_settings)
        , sandbox_board(a_settings, a_live_board)
        , best_play_board(a_settings, a_live_board)
        , hand(a_settings, a_hand)
        , direction(a_direction)
        , line_index(a_line_index)
        , relevant_rows(a_relevant_rows)
        , relevant_columns(a_relevant_columns)
        , touch_evaluation(a_touch_evaluation)
    {
        initialise();
    }
    
    PlayerAIWorker::~PlayerAIWorker()
    { }
    
    int PlayerAIWorker::getScore()
    {
        return best_play_board.getProposedPlayPoints();
    }
    
    std::vector<QPoint> PlayerAIWorker::getLetterPositionsInHand()
    {
        VirtualTile* tile;
        Letter* letter;

        auto proposed_tiles = best_play_board.getProposedLetters();
        std::vector<QPoint> positions;

        positions.resize(proposed_tiles.size());
        
        for (int row = 0; row < hand.getGridDimensions().height(); ++row)
        {
            for (int column = 0; column < hand.getGridDimensions().width(); ++column)
            {
                tile = hand.getTileAtPosition(column, row);
                letter = tile->getLetter();

                for (size_t index = 0; index < proposed_tiles.size(); ++index)
                {
                    if (proposed_tiles[index]->getLetter() == letter)
                    {
                        positions[index] = QPoint{column, row};
                    }
                }
            }
        }

        return positions;
    }
    
    std::vector<QPoint> PlayerAIWorker::getLetterPositionsOnBoard()
    {
        std::vector<QPoint> positions;

        for (auto tile : best_play_board.getProposedLetters())
        {
            positions.push_back(tile->getGridPosition());
        }

        return positions;
    }
    
    std::vector<QChar> PlayerAIWorker::getWildcardLetters()
    {
        return best_play_wildcard_letters;
    }
    
    void PlayerAIWorker::execute()
    {
        findPlayInLine();
        emit finished();
        return;
    }
    
    void PlayerAIWorker::cancel()
    {
        cancelled = true;
        return;
    }
    
    void PlayerAIWorker::initialise()
    {
        VirtualTile* tile;
        Letter* letter;

        available_letters.clear();
        
        for (int row = 0; row < hand.getGridDimensions().height(); ++row)
        {
            for (int column = 0; column < hand.getGridDimensions().width(); ++column)
            {
                tile = hand.getTileAtPosition(column, row);
                letter = tile->getLetter();

                if (letter != nullptr)
                {
                    available_letters.push_back(letter);
                    ++available_letter_count;
                }
            }
        }

        return;
    }
    
    void PlayerAIWorker::findPlayInLine()
    {
        switch (direction)
        {
            case Direction::Horisontal:
            {
                if (relevant_rows.contains(line_index))
                {
                    findPlayInHorisontalLine();
                }

                break;
            }
            case Direction::Vertical:
            {
                if (relevant_columns.contains(line_index))
                {
                    findPlayInVerticalLine();
                }

                break;
            }
        }

        return;
    }
    
    void PlayerAIWorker::findPlayInHorisontalLine()
    {
        for (int column = 0; column < sandbox_board.getGridDimensionInDirection(Direction::Horisontal); ++column)
        {
            findPlayRecursively(column, line_index);
        }

        return;
    }
    
    void PlayerAIWorker::findPlayInVerticalLine()
    {
        for (int row = 0; row < sandbox_board.getGridDimensionInDirection(Direction::Vertical); ++row)
        {
            findPlayRecursively(line_index, row);
        }

        return;
    }
    
    // For horisontal, fixed index = row, variable index = column
    // For vertical, fixed index = column, variable index = row
    void PlayerAIWorker::findPlayRecursively(int a_column, int a_row)
    {
        if (cancelled)
        {
            return;
        }

        if (indexOutOfBounds(a_column, a_row))
        {
            return;
        }

        VirtualTile* tile = sandbox_board.getTileAtPosition(a_column, a_row);
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
                    auto all_letters = settings->getLanguage()->getLetterList();

                    for (auto letter_option : all_letters)
                    {
                        letter->setWildcardText(letter_option.letter);
                        tryLetterAndRecurse(letter, tile, a_column, a_row);
                    }
                }
                else
                {
                    tryLetterAndRecurse(letter, tile, a_column, a_row);
                }
            }
        }
        else
        {
            recurse(a_column, a_row);
        }

        return;
    }
    
    void PlayerAIWorker::tryLetterAndRecurse(Letter*& a_letter, VirtualTile* a_tile, int a_column, int a_row)
    {
        a_tile->placeLetter(a_letter);
        a_letter = nullptr;
        sandbox_board.proposeLetter(a_tile);
        --available_letter_count;
        touch_count += touch_evaluation[a_column][a_row];

        if (touch_count > 0)
        {
            updateBestPlay();
        }

        if (available_letter_count > 0)
        {
            recurse(a_column, a_row);
        }

        touch_count -= touch_evaluation[a_column][a_row];
        ++available_letter_count;
        sandbox_board.unproposeLetter(a_tile);
        a_letter = a_tile->removeLetter();

        return;
    }
    
    void PlayerAIWorker::recurse(int a_column, int a_row)
    {
        switch (direction)
        {
            case Direction::Horisontal:
            {
                return findPlayRecursively(a_column + 1, a_row);
            }
            case Direction::Vertical:
            {
                return findPlayRecursively(a_column, a_row + 1);
            }
        }
    }
    
    bool PlayerAIWorker::indexOutOfBounds(int a_column, int a_row)
    {
        return a_column < 0
            || a_row < 0
            || a_column >= sandbox_board.getGridDimensionInDirection(Direction::Horisontal)
            || a_row >= sandbox_board.getGridDimensionInDirection(Direction::Vertical);
    }
    
    void PlayerAIWorker::updateBestPlay()
    {
        best_play_board.evaluateProposedPlay(false, true, true);
        sandbox_board.evaluateProposedPlay(false, true, true);

        if (!sandbox_board.isProposedPlayValid())
        {
            return;
        }

        if (sandbox_board.getProposedPlayPoints() > best_play_board.getProposedPlayPoints())
        {
            best_play_board.setWithBoard(&sandbox_board);
            best_play_board.importProposedLetters(sandbox_board.getProposedLetters());
            setBestPlayWildcardLetters();
        }
    }
    
    void PlayerAIWorker::setBestPlayWildcardLetters()
    {
        Letter* letter;
        QChar letter_text;
        
        best_play_wildcard_letters.clear();

        for (auto tile : sandbox_board.getProposedLetters())
        {
            letter = tile->getLetter();
            
            if (letter->getType() == LetterType::Wildcard)
            {
                letter_text = letter->getWildcardText();
            }
            else
            {
                letter_text = QChar{};
            }

            best_play_wildcard_letters.push_back(letter_text);
        }

        return;
    }


}