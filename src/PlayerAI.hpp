#ifndef __PLAYERAI_H__
#define __PLAYERAI_H__

#include <algorithm>
#include <array>
#include <set>
#include <vector>

#include <QChar>
#include <QColor>
#include <QDateTime>
#include <QPoint>
#include <QString>
#include <QThread>

#include "ForwardDeclarations.hpp"

#include "Letter.hpp"
#include "LetterPool.hpp"
#include "Player.hpp"
#include "PlayerAIWorker.hpp"
#include "PlayerSettings.hpp"
#include "RenderedTile.hpp"
#include "Settings.hpp"
#include "VirtualBoard.hpp"
#include "VirtualTile.hpp"

#include "Direction.hpp"
#include "LetterType.hpp"
#include "TileNeighbours.hpp"

namespace wf
{
    struct QCharHash
    {
        size_t operator()(const QChar& a_char) const
        {
            return qHash(a_char);
        }
    };
    
    class PlayerAI : public Player
    {
        Q_OBJECT
        
        public:
            PlayerAI(
                QString a_display_name,
                QColor a_color,
                Settings* a_settings,
                RenderedTile* a_selection,
                RenderedBoard* a_board,
                LetterPool* a_letter_pool,
                int a_index);
            ~PlayerAI();

            int getDifficulty() const;
            void fillHand(LetterPool* a_letter_pool);

        public slots:
            void playIfTurn();
            void cancelTurn();
            void bestPlayFound();
        
        signals:
            void playComplete();
            void letterPlaced();
            void startSwap();
            void letterMarkedForSwap();
            void swapComplete();
            void passTurn();
            void startWorkers();
            void cleanup();

        private:
            void startTurn();
            void endTurn();
            void startOfTurnSetup();
            void findBestPlay();
            void executeBestPlay();
            void updateRelevantLines();
            bool isLineRelevant(Direction a_direction, int a_index);
            void initialiseTouchEvaluation(VirtualBoard* a_board);
            void evaluateBoard(VirtualBoard* a_board);
            int evaluateTile(VirtualTile* a_tile);
            void swapAllLetters();
            void initialiseWorkerThreads();
            void deleteWorkers();
            int countWildcardsInHand();

            bool cancelled = false;
            RenderedBoard* live_board;
            std::set<int> relevant_collumns;
            std::set<int> relevant_rows;
            std::vector<std::vector<int>> touch_evaluation;
            LetterPool* letter_pool;
            std::vector<QThread*> worker_threads;
            std::vector<PlayerAIWorker*> workers;
            int best_play_score = 0;
            int best_play_index = 0;
            size_t finished_workers = 0;
            qint64 turn_start_time;
            qint64 turn_end_time;
            int difficulty;
    };
}

#endif // __PLAYERAI_H__