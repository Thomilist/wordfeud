#include "PlayerAI.hpp"


namespace wf
{
    PlayerAI::PlayerAI(
        QString a_display_name,
        QColor a_color,
        Settings* a_settings,
        RenderedTile* a_selection,
        RenderedBoard* a_board,
        LetterPool* a_letter_pool,
        int a_index)
        : Player(
            a_display_name,
            PlayerType::AI,
            a_color,
            a_settings,
            a_selection,
            a_index)
        , live_board(a_board)
        , letter_pool(a_letter_pool)
        , difficulty(a_settings->getPlayer(a_index)->getAIDifficulty())
    {
        initialiseTouchEvaluation(live_board);
        initialiseWorkerThreads();
    }
    
    PlayerAI::~PlayerAI()
    {
        deleteWorkers();
        emit cleanup();
    }
    
    int PlayerAI::getDifficulty() const
    {
        return difficulty;
    }
    
    void PlayerAI::fillHand(LetterPool* a_letter_pool)
    {
        Player::fillHand(a_letter_pool);

        // If possible, PlayerAI should have no more than one wildcard letter in hand,
        // since these add a lot of computation time to the turn
        RenderedTile* tile;
        Letter* letter;
        bool wildcard_swapped;

        while (countWildcardsInHand() > 1
            && a_letter_pool->getRemainingWildcardCount() < a_letter_pool->getRemainingCount())
        {
            wildcard_swapped = false;
            
            for (int row = 0; row < getHand()->getGridDimensions().height() && !wildcard_swapped; ++row)
            {
                for (int column = 0; column < getHand()->getGridDimensions().width() && !wildcard_swapped; ++column)
                {
                    tile = getHand()->getTileAtPosition(column, row);
                    letter = tile->getLetter();

                    if (letter != nullptr && letter->getType() == LetterType::Wildcard)
                    {
                        letter = tile->removeLetter();
                        Player::fillHand(a_letter_pool);
                        a_letter_pool->insertLetter(letter);
                        wildcard_swapped = true;
                    }
                }
            }
        }

        return;
    }
    
    void PlayerAI::playIfTurn()
    {
        if (hasTurn())
        {
            startTurn();
        }

        return;
    }
    
    void PlayerAI::cancelTurn()
    {
        cancelled = true;

        for (auto worker : workers)
        {
            worker->cancel();
        }

        deleteWorkers();
        emit cleanup();

        return;
    }
    
    void PlayerAI::bestPlayFound()
    {
        ++finished_workers;

        if (cancelled)
        {
            return;
        }

        if (finished_workers < workers.size())
        {
            return;
        }

        int worker_index = 0;
        int new_score = 0;

        for (auto worker : workers)
        {
            new_score = worker->getScore();

            if (new_score > best_play_score)
            {
                best_play_score = new_score;
                best_play_index = worker_index;
            }

            ++worker_index;
        }

        turn_end_time = QDateTime::currentMSecsSinceEpoch();
        qint64 delta_time = turn_end_time - turn_start_time;
        qint64 minimum_time = settings->getMinimumAITurnTime() * 1000;

        if (minimum_time > delta_time)
        {
            QThread::msleep(minimum_time - delta_time);
        }
        
        executeBestPlay();

        if (cancelled)
        {
            return;
        }

        endTurn();

        return;
    }
    
    void PlayerAI::startTurn()
    {
        turn_start_time = QDateTime::currentMSecsSinceEpoch();
        
        startOfTurnSetup();
        findBestPlay();

        return;
    }
    
    void PlayerAI::endTurn()
    {
        if (best_play_score > 0)
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

        deleteWorkers();

        return;
    }
    
    void PlayerAI::startOfTurnSetup()
    {
        updateRelevantLines();
        evaluateBoard(live_board);
        best_play_score = 0;
        finished_workers = 0;

        return;
    }
    
    void PlayerAI::findBestPlay()
    {
        PlayerAIWorker* worker;
        
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
                worker = new PlayerAIWorker
                (
                    settings,
                    live_board,
                    getHand(),
                    letter_pool,
                    direction,
                    index,
                    relevant_rows,
                    relevant_collumns,
                    touch_evaluation,
                    difficulty
                );

                worker->moveToThread(worker_threads[index]);

                connect(this, &PlayerAI::startWorkers, worker, &PlayerAIWorker::execute);
                connect(worker, &PlayerAIWorker::finished, this, &PlayerAI::bestPlayFound);
                
                workers.push_back(worker);
            }
        }

        emit startWorkers();

        return;
    }
    
    void PlayerAI::executeBestPlay()
    {
        std::vector<QPoint> positions_in_hand = workers[best_play_index]->getLetterPositionsInHand();
        std::vector<QPoint> positions_on_board = workers[best_play_index]->getLetterPositionsOnBoard();
        std::vector<Letter*> proposed_letters = workers[best_play_index]->getProposedLetters();

        if (positions_in_hand.size() == 0)
        {
            return;
        }

        if (positions_in_hand.size() != positions_on_board.size())
        {
            return;
        }

        QPoint hand_position;
        QPoint board_position;

        VirtualTile* tile;
        Letter* letter;
        
        for (size_t index = 0; index < positions_in_hand.size(); ++index)
        {
            if (index > 0)
            {
                QThread::msleep(settings->getAILetterPlacingDelay());
            }

            if (cancelled)
            {
                return;
            }
            
            hand_position = positions_in_hand[index];
            board_position = positions_on_board[index];

            tile = live_board->getTileAtPosition(board_position);
            letter = getHand()->removeLetter(hand_position);

            if (letter->getType() == LetterType::Wildcard)
            {
                letter = proposed_letters[index];
                letter->setOwner(this);
            }

            tile->placeLetter(letter);
            emit letterPlaced();
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
    
    void PlayerAI::initialiseTouchEvaluation(VirtualBoard* a_board)
    {
        touch_evaluation.reserve(a_board->getGridDimensions().width());
        std::vector<int> collumn_evaluation;
        collumn_evaluation.reserve(a_board->getGridDimensions().height());

        for (int column = 0; column < a_board->getGridDimensions().width(); ++column)
        {
            collumn_evaluation.clear();
            
            for (int row = 0; row < a_board->getGridDimensions().height(); ++row)
            {
                collumn_evaluation.push_back(0);
            }

            touch_evaluation.push_back(collumn_evaluation);
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
                touch_evaluation[column][row] = evaluateTile(tile);
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
                if (cancelled)
                {
                    return;
                }
                
                tile = getHand()->getTileAtPosition(column, row);
                tile->setSwapMarking(true);
                emit tile->markForSwap(tile);
                emit letterMarkedForSwap();

                QThread::msleep(settings->getAILetterPlacingDelay());
            }
        }

        return;
    }
    
    void PlayerAI::initialiseWorkerThreads()
    {
        QThread* worker_thread;
        
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
                worker_thread = new QThread;
                connect(this, &PlayerAI::cleanup, worker_thread, &QThread::quit);
                connect(worker_thread, &QThread::finished, worker_thread, &QThread::deleteLater);
                worker_thread->start();
                worker_threads.push_back(worker_thread);
            }
        }

        return;
    }
    
    void PlayerAI::deleteWorkers()
    {
        for (size_t index = 0; index < workers.size(); ++index)
        {
            workers[index]->deleteLater();
        }

        workers.clear();

        return;
    }
    
    int PlayerAI::countWildcardsInHand()
    {
        RenderedTile* tile;
        Letter* letter;
        int count = 0;

        for (int row = 0; row < getHand()->getGridDimensions().height(); ++row)
        {
            for (int column = 0; column < getHand()->getGridDimensions().width(); ++column)
            {
                tile = getHand()->getTileAtPosition(column, row);
                letter = tile->getLetter();

                if (letter != nullptr && letter->getType() == LetterType::Wildcard)
                {
                    ++count;
                }
            }
        }

        return count;
    }
}