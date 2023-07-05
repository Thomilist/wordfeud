#include "PlayerAIWorker.hpp"


namespace wf
{
    PlayerAIWorker::PlayerAIWorker(
        Settings* a_settings,
        VirtualBoard* a_live_board,
        VirtualBoard* a_hand,
        LetterPool* a_letter_pool,
        Direction a_direction,
        int a_line_index,
        std::vector<std::vector<int>> a_touch_evaluation,
        int a_difficulty)
        : settings(a_settings)
        , sandbox_board(a_settings, a_live_board)
        , best_play_board(a_settings, a_live_board)
        , hand(a_settings, a_hand)
        , letter_pool(a_letter_pool)
        , direction(a_direction)
        , line_index(a_line_index)
        , touch_evaluation(a_touch_evaluation)
        , rng(random_seed())
        , random_distribution(0, 50000)
        , difficulty(a_difficulty)
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
        Letter* letter_in_hand;
        Letter* proposed_letter;

        auto proposed_tiles = best_play_board.getProposedLetters();
        std::vector<QPoint> positions;

        positions.resize(proposed_tiles.size());
        
        for (int row = 0; row < hand.getGridDimensions().height(); ++row)
        {
            for (int column = 0; column < hand.getGridDimensions().width(); ++column)
            {
                tile = hand.getTileAtPosition(column, row);
                letter_in_hand = tile->getLetter();

                if (letter_in_hand == nullptr)
                {
                    continue;
                }

                for (size_t index = 0; index < proposed_tiles.size(); ++index)
                {
                    proposed_letter = proposed_tiles[index]->getLetter();
                    
                    if (proposed_letter == letter_in_hand)
                    {
                        positions[index] = QPoint{column, row};
                    }
                    else if (   letter_in_hand->getType() == LetterType::Wildcard
                            &&  wildcard_substitutes.contains(letter_in_hand))
                    {
                        std::vector<Letter*> substitutes = wildcard_substitutes[letter_in_hand];
                        
                        if (std::find(substitutes.begin(), substitutes.end(), proposed_letter) != substitutes.end())
                        {
                            positions[index] = QPoint{column, row};
                        }
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

    std::vector<Letter*> PlayerAIWorker::getProposedLetters()
    {
        std::vector<Letter*> letters;

        for (auto tile : best_play_board.getProposedLetters())
        {
            letters.push_back(tile->getLetter());
        }

        return letters;
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

        initialiseWildcardSubstitutes();

        return;
    }
    
    void PlayerAIWorker::findPlayInLine()
    {
        switch (direction)
        {
            case Direction::Horisontal:
            {
                findPlayInHorisontalLine();
                break;
            }
            case Direction::Vertical:
            {
                findPlayInVerticalLine();
                break;
            }
        }

        return;
    }
    
    void PlayerAIWorker::findPlayInHorisontalLine()
    {
        for (int column = 0; column < sandbox_board.getGridDimensionInDirection(Direction::Horisontal); ++column)
        {
            startSearchOnTile(column, line_index);
        }

        return;
    }
    
    void PlayerAIWorker::findPlayInVerticalLine()
    {
        for (int row = 0; row < sandbox_board.getGridDimensionInDirection(Direction::Vertical); ++row)
        {
            startSearchOnTile(line_index, row);
        }

        return;
    }
    
    void PlayerAIWorker::startSearchOnTile(int a_column, int a_row)
    {
        if (!isPositionRelevant(a_column, a_row))
        {
            return;
        }
        
        current_combination.clear();
        tried_combinations.clear();
        tried_combinations.reserve(200000);
        findPlayRecursively(a_column, a_row);
        return;
    }
    
    // For horisontal, fixed index = row, variable index = column.
    // For vertical, fixed index = column, variable index = row.
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
                    // Set letter to nullptr to mark it unavailable
                    Letter* letter_holder = letter;
                    letter = nullptr;

                    for (auto& letter_option : wildcard_substitutes[letter_holder])
                    {
                        if (cancelled)
                        {
                            break;
                        }
                        
                        tryLetterAndRecurse(letter_option, tile, a_column, a_row);
                    }

                    letter = letter_holder;
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
        current_combination.push_back(a_letter->getText());
        a_tile->placeLetter(a_letter);
        a_letter = nullptr;
        sandbox_board.proposeLetter(a_tile);
        --available_letter_count;
        touch_count += touch_evaluation[a_column][a_row];

        bool tried_already = isCombinationTriedAlready();

        if (touch_count > 0 && !tried_already)
        {
            updateBestPlay();
        }

        if (available_letter_count > 0 && !tried_already)
        {
            recurse(a_column, a_row);
        }

        touch_count -= touch_evaluation[a_column][a_row];
        ++available_letter_count;
        sandbox_board.unproposeLetter(a_tile);
        a_letter = a_tile->removeLetter();
        current_combination.chop(1);

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
            if (    difficulty == settings->getMaximumAIDifficulty()
                ||  rollDifficultyDice())
            {
                best_play_board.setWithBoard(&sandbox_board);
                best_play_board.importProposedLetters(sandbox_board.getProposedLetters());
            }
        }

        return;
    }
    
    bool PlayerAIWorker::rollDifficultyDice()
    {
        return random_distribution(rng) < (difficulty * difficulty * 1000) / (sandbox_board.getProposedPlayPoints() + 1);
    }
    
    void PlayerAIWorker::initialiseWildcardSubstitutes()
    {
        VirtualTile* tile;
        Letter* letter;
        
        for (int row = 0; row < hand.getGridDimensions().height(); ++row)
        {
            for (int column = 0; column < hand.getGridDimensions().width(); ++column)
            {
                tile = hand.getTileAtPosition(column, row);
                letter = tile->getLetter();

                if (letter != nullptr && letter->getType() == LetterType::Wildcard)
                {
                    wildcard_substitutes[letter] = letter_pool->getWildcardSubstitutes(letter);
                }
            }
        }

        return;
    }
    
    bool PlayerAIWorker::isCombinationTriedAlready()
    {
        bool tried_already = tried_combinations.contains(current_combination);

        if (!tried_already)
        {
            tried_combinations.insert(current_combination);
        }

        return tried_already;
    }
    
    bool PlayerAIWorker::isPositionRelevant(int a_column, int a_row)
    {
        for (int reach = available_letter_count; reach > 0; --reach)
        {
            if (indexOutOfBounds(a_column, a_row))
            {
                return false;
            }
            
            if (touch_evaluation[a_column][a_row] > 0)
            {
                return true;
            }

            switch (direction)
            {
                case Direction::Horisontal:
                {
                    ++a_column;
                    break;
                }
                case Direction::Vertical:
                {
                    ++a_row;
                    break;
                }
            }
        }

        return false;
    }
}