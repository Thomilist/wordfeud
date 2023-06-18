#include "PlayerAI.hpp"


namespace wf
{
    PlayerAI::PlayerAI(
        QString a_display_name,
        QColor a_color,
        Settings* a_settings,
        RenderedTile* a_selection,
        RenderedBoard* a_board,
        LetterPool* a_letter_pool)
        : Player(
            a_display_name,
            PlayerType::AI,
            a_color,
            a_settings,
            a_selection)
        , live_board(a_board)
        , best_sandbox(
            Sandbox(
                VirtualBoard(settings, live_board),
                VirtualBoard(settings, live_board),
                std::vector<QChar>(),
                std::vector<Letter*>(),
                0,
                0))
        , letter_pool(a_letter_pool)
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

        if (cancelled)
        {
            return;
        }

        endTurn();
        
        return;
    }
    
    void PlayerAI::endTurn()
    {
        if (best_sandbox.best_play.getProposedPlayPoints() > 0)
        {
            emit playComplete();
        }
        else if (letter_pool->getRemainingCount() > 0)
        {
            emit startSwap();
            swapAllLetters();
            emit swapComplete();
        }
        else
        {
            emit passTurn();
        }

        return;
    }
    
    void PlayerAI::startOfTurnSetup()
    {
        updateRelevantLines();
        evaluateBoard(live_board);
        initialiseSandboxes();

        return;
    }
    
    void PlayerAI::fetchAvailableLetters(Sandbox* a_sandbox)
    {
        a_sandbox->available_letters.clear();
        a_sandbox->available_letter_count = 0;
        Letter* letter;

        for (int row = 0; row < getHand()->getGridDimensions().height(); ++row)
        {
            for (int column = 0; column < getHand()->getGridDimensions().width(); ++column)
            {
                letter = getHand()->getTileAtPosition(column, row)->getLetter();

                if (letter != nullptr)
                {
                    a_sandbox->available_letters.push_back(letter);
                    ++a_sandbox->available_letter_count;
                }
            }
        }

        return;
    }
    
    void PlayerAI::initialiseSandboxes()
    {
        int line_count = live_board->getGridDimensions().width() + live_board->getGridDimensions().height();
        
        sandboxes.clear();
        sandboxes.reserve(line_count);

        for (int index = 0; index < line_count; ++index)
        {
            sandboxes.push_back
            (
                Sandbox
                    (
                        VirtualBoard(settings, live_board),
                        VirtualBoard(settings, live_board),
                        std::vector<QChar>(),
                        std::vector<Letter*>(),
                        0,
                        0
                    )
            );
        }

        best_sandbox.best_play.clearProposed();
        best_sandbox.best_play.evaluateProposedPlay();

        return;
    }
    
    void PlayerAI::findBestPlay()
    {
        int sandbox_index = 0;
        
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
                fetchAvailableLetters(&sandboxes[sandbox_index]);
                findPlayInLine(&sandboxes[sandbox_index], direction, index);
                ++sandbox_index;
            }
        }

        for (auto& sandbox : sandboxes)
        {
            updateBestPlay(&best_sandbox, &sandbox);
        }

        return;
    }
    
    void PlayerAI::findPlayInLine(Sandbox* a_sandbox, Direction a_direction, int a_index)
    {
        switch (a_direction)
        {
            case Direction::Horisontal:
            {
                if (relevant_rows.contains(a_index))
                {
                    findPlayInHorisontalLine(a_sandbox, a_index);
                }

                break;
            }
            case Direction::Vertical:
            {
                if (relevant_collumns.contains(a_index))
                {
                    findPlayInVerticalLine(a_sandbox, a_index);
                }
                
                break;
            }
        }

        return;
    }
    
    void PlayerAI::findPlayInHorisontalLine(Sandbox* a_sandbox, int a_row)
    {
        for (int column = 0; column < live_board->getGridDimensionInDirection(Direction::Horisontal); ++column)
        {
            findPlayRecursively(a_sandbox, column, a_row, Direction::Horisontal);
        }

        return;
    }
    
    void PlayerAI::findPlayInVerticalLine(Sandbox* a_sandbox, int a_column)
    {
        for (int row = 0; row < live_board->getGridDimensionInDirection(Direction::Vertical); ++row)
        {
            findPlayRecursively(a_sandbox, a_column, row, Direction::Vertical);
        }

        return;
    }
    
    // For horisontal, fixed index = row, variable index = column
    // For vertical, fixed index = column, variable index = row
    void PlayerAI::findPlayRecursively(Sandbox* a_sandbox, int a_column, int a_row, Direction a_direction)
    {
        if (cancelled)
        {
            return;
        }
        
        if (indexOutOfBounds(a_sandbox, a_column, a_row))
        {
            return;
        }

        VirtualTile* tile = a_sandbox->board.getTileAtPosition(a_column, a_row);
        bool tile_available = tile->getLetter() == nullptr;

        if (tile_available)
        {
            for (auto& letter : a_sandbox->available_letters)
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
                        tryLetterAndRecurse(a_sandbox, letter, tile, a_column, a_row, a_direction);
                    }
                }
                else
                {
                    tryLetterAndRecurse(a_sandbox, letter, tile, a_column, a_row, a_direction);
                }
            }
        }
        else
        {
            recurse(a_sandbox, a_column, a_row, a_direction);
        }

        return;
    }
    
    void PlayerAI::tryLetterAndRecurse(Sandbox* a_sandbox, Letter*& a_letter, VirtualTile* a_tile, int a_column, int a_row, Direction a_direction)
    {
        a_tile->placeLetter(a_letter);
        a_letter = nullptr;
        a_sandbox->board.proposeLetter(a_tile);
        --a_sandbox->available_letter_count;
        a_sandbox->touch_count += board_evaluation[a_column][a_row];

        if (a_sandbox->touch_count > 0)
        {
            updateBestPlay(a_sandbox);
        }

        if (a_sandbox->available_letter_count > 0)
        {
            recurse(a_sandbox, a_column, a_row, a_direction);
        }
        
        a_letter = a_tile->removeLetter();
        a_sandbox->board.unproposeLetter(a_tile);
        ++a_sandbox->available_letter_count;
        a_sandbox->touch_count -= board_evaluation[a_column][a_row];

        return;
    }
    
    void PlayerAI::recurse(Sandbox* a_sandbox, int a_column, int a_row, Direction a_direction)
    {
        switch (a_direction)
        {
            case Direction::Horisontal:
            {
                return findPlayRecursively(a_sandbox, a_column + 1, a_row, a_direction);
                break;
            }
            case Direction::Vertical:
            {
                return findPlayRecursively(a_sandbox, a_column, a_row + 1, a_direction);
                break;
            }
        }

        return;
    }
    
    bool PlayerAI::indexOutOfBounds(Sandbox* a_sandbox, int a_column, int a_row)
    {
        return a_column >= a_sandbox->board.getGridDimensionInDirection(Direction::Horisontal) || a_row >= a_sandbox->board.getGridDimensionInDirection(Direction::Vertical);
    }
    
    void PlayerAI::updateBestPlay(Sandbox* a_sandbox)
    {
        updateBestPlay(a_sandbox, a_sandbox);
        return;
    }

    void PlayerAI::updateBestPlay(Sandbox* a_sandbox_with_best, Sandbox* a_sandbox_with_new)
    {
        a_sandbox_with_best->best_play.evaluateProposedPlay(false, true, true);
        a_sandbox_with_new->board.evaluateProposedPlay(false, true, true);
        
        if (!a_sandbox_with_new->board.isProposedPlayValid())
        {
            return;
        }

        int current_best_points = a_sandbox_with_best->best_play.getProposedPlayPoints();
        int new_play_points = 0;

        if (a_sandbox_with_best == a_sandbox_with_new)
        {
            new_play_points = a_sandbox_with_new->board.getProposedPlayPoints();
        }
        else
        {
            new_play_points = a_sandbox_with_new->best_play.getProposedPlayPoints();
        }

        if (new_play_points > current_best_points)
        {
            if (a_sandbox_with_best == a_sandbox_with_new)
            {
                a_sandbox_with_best->best_play.setWithBoard(&a_sandbox_with_new->board);
                a_sandbox_with_best->best_play.importProposedLetters(a_sandbox_with_new->board.getProposedLetters());
                setBestPlayWildcardLetters(a_sandbox_with_best, a_sandbox_with_new->board.getProposedLetters());
            }
            else
            {
                a_sandbox_with_best->best_play.setWithBoard(&a_sandbox_with_new->best_play);
                a_sandbox_with_best->best_play.importProposedLetters(a_sandbox_with_new->best_play.getProposedLetters());
                setBestPlayWildcardLetters(a_sandbox_with_best, a_sandbox_with_new->best_play_wildcard_letters);
            }
        }

        return;
    }
    
    void PlayerAI::executeBestPlay()
    {
        best_sandbox.best_play.evaluateProposedPlay(true);
        
        QPoint position;
        Letter* letter;
        VirtualTile* live_tile;
        int index = 0;
        
        for (auto local_tile : best_sandbox.best_play.getProposedLetters())
        {
            QThread::msleep(100);

            if (cancelled)
            {
                return;
            }
            
            position = local_tile->getGridPosition();
            letter = local_tile->getLetter();
            live_tile = live_board->getTileAtPosition(position.x(), position.y());

            // Wildcard letters must have their letter contents restored
            if (letter->getType() == LetterType::Wildcard)
            {
                letter->setWildcardText(best_sandbox.best_play_wildcard_letters.at(index));
            }

            // The live board holds rendered tiles, which automatically propose a letter when placed
            live_tile->placeLetter(letter);

            // Remove letter from hand
            bool letter_removed = false;
            VirtualTile* tile;

            for (int row = 0; row < getHand()->getGridDimensions().height() && !letter_removed; ++row)
            {
                for (int column = 0; column < getHand()->getGridDimensions().width() && !letter_removed; ++column)
                {
                    tile = getHand()->getTileAtPosition(column, row);
                    
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

        return;
    }
    
    void PlayerAI::setBestPlayWildcardLetters(Sandbox* a_best_sandbox, std::vector<VirtualTile*> a_proposed_tiles)
    {
        std::vector<QChar> wildcard_letters;
        QChar letter;

        for (auto tile : a_proposed_tiles)
        {
            if (tile->getLetter()->getType() == LetterType::Wildcard)
            {
                letter = tile->getLetter()->getWildcardText();
            }
            else
            {
                letter = QChar{};
            }

            wildcard_letters.push_back(letter);
        }

        setBestPlayWildcardLetters(a_best_sandbox, wildcard_letters);

        return;
    }
    
    void PlayerAI::setBestPlayWildcardLetters(Sandbox* a_best_sandbox, std::vector<QChar> a_proposed_letters)
    {
        a_best_sandbox->best_play_wildcard_letters.clear();

        for (auto letter : a_proposed_letters)
        {
            a_best_sandbox->best_play_wildcard_letters.push_back(letter);
        }

        return;
    }
    
    void PlayerAI::updateRelevantLines()
    {
        relevant_collumns.clear();
        relevant_rows.clear();

        VirtualTile* tile;

        for (int column = 0; column < live_board->getGridDimensions().width(); ++column)
        {
            for (int row = 0; row < live_board->getGridDimensions().height(); ++row)
            {
                tile = live_board->getTileAtPosition(column, row);

                // A column or row is relevant if it is the start tile or a letter is on or adjacent to it
                if (tile->getModifier()->getType() == ModifierType::Start)
                {
                    relevant_collumns.insert(column);
                    relevant_rows.insert(row);
                }
                else if (tile->getLetter() != nullptr)
                {
                    // Add column before, at and after
                    if (column - 1 >= 0)
                    {
                        relevant_collumns.insert(column - 1);
                    }

                    relevant_collumns.insert(column);

                    if (column + 1 <= live_board->getGridDimensions().width())
                    {
                        relevant_collumns.insert(column + 1);
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

        for (int column = 0; column < a_board->getGridDimensions().width(); ++column)
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

        for (int column = 0; column < a_board->getGridDimensions().width(); ++column)
        {
            for (int row = 0; row < a_board->getGridDimensions().height(); ++row)
            {
                tile = a_board->getTileAtPosition(column, row);
                board_evaluation[column][row] = evaluateTile(tile);
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
    
    void PlayerAI::swapAllLetters()
    {
        RenderedTile* tile;
        
        for (int row = 0; row < getHand()->getGridDimensions().height(); ++row)
        {
            for (int column = 0; column < getHand()->getGridDimensions().width(); ++column)
            {
                tile = getHand()->getTileAtPosition(column, row);
                tile->setSwapMarking(true);
                emit tile->markForSwap(tile);
                emit letterMarkedForSwap();
            }
        }

        return;
    }
}