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
        , letter_pool(a_letter_pool)
    {
        initialiseTouchEvaluation(live_board);
        initialiseWorkerThreads();
    }
    
    PlayerAI::~PlayerAI()
    {
        deleteWorkers();
        emit cleanup();
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
        emit cancelWork();

        while (finished_workers < workers.size())
        {
            QThread::msleep(10);
        }

        deleteWorkers();
        emit cleanup();

        return;
    }
    
    void PlayerAI::bestPlayFound()
    {
        ++finished_workers;

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

        return;
    }
    
    void PlayerAI::startOfTurnSetup()
    {
        updateRelevantLines();
        evaluateBoard(live_board);
        deleteWorkers();
        worker_threads.clear();
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
                    direction,
                    index,
                    relevant_rows,
                    relevant_collumns,
                    touch_evaluation
                );

                worker->moveToThread(worker_threads[index]);

                connect(this, &PlayerAI::startWorkers, worker, &PlayerAIWorker::execute);
                connect(this, &PlayerAI::cancelWork, worker, &PlayerAIWorker::cancel);
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
        std::vector<QChar> wildcard_letters = workers[best_play_index]->getWildcardLetters();

        if (positions_in_hand.size() == 0)
        {
            return;
        }

        QPoint hand_position;
        QPoint board_position;
        QChar wildcard_letter;

        VirtualTile* tile;
        Letter* letter;
        
        for (size_t index = 0; index < positions_in_hand.size(); ++index)
        {
            QThread::msleep(100);

            if (cancelled)
            {
                return;
            }
            
            hand_position = positions_in_hand[index];
            board_position = positions_on_board[index];
            wildcard_letter = wildcard_letters[index];

            tile = live_board->getTileAtPosition(board_position);
            letter = getHand()->removeLetter(hand_position);

            if (letter->getType() == LetterType::Wildcard)
            {
                letter->setWildcardText(wildcard_letter);
            }

            tile->placeLetter(letter);
            live_board->proposeLetter(tile);

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
                tile = getHand()->getTileAtPosition(column, row);
                tile->setSwapMarking(true);
                emit tile->markForSwap(tile);
                emit letterMarkedForSwap();
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
}