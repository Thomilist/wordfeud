#ifndef __PLAYERAIWORKER_H__
#define __PLAYERAIWORKER_H__

#include <algorithm>
#include <map>
#include <random>
#include <stack>
#include <unordered_set>
#include <vector>

#include <QChar>
#include <QObject>
#include <QPoint>
#include <QString>
#include <QStringBuilder>

#include "Letter.hpp"
#include "LetterPool.hpp"
#include "Settings.hpp"
#include "VirtualBoard.hpp"
#include "VirtualTile.hpp"

#include "Direction.hpp"
#include "LetterType.hpp"

namespace wf
{
    class PlayerAIWorker : public QObject
    {
        Q_OBJECT
        
        public:
            PlayerAIWorker(
                Settings* a_settings,
                VirtualBoard* a_live_board,
                VirtualBoard* a_hand,
                LetterPool* a_letter_pool,
                Direction a_direction,
                int a_line_index,
                std::vector<std::vector<int>> a_touch_evaluation,
                int a_difficulty);
            ~PlayerAIWorker();

            int getScore();
            std::vector<QPoint> getLetterPositionsInHand();
            std::vector<QPoint> getLetterPositionsOnBoard();
            std::vector<Letter*> getProposedLetters();

        public slots:
            void execute();
            void cancel();
        
        signals:
            void finished();

        private:
            void initialise();
            void findPlayInLine();
            void findPlayInHorisontalLine();
            void findPlayInVerticalLine();
            void startSearchOnTile(int a_column, int a_row);
            void findPlayRecursively(int a_column, int a_row);
            void tryLetterAndRecurse(Letter*& a_letter, VirtualTile* a_tile, int a_column, int a_row);
            void recurse(int a_column, int a_row);
            bool indexOutOfBounds(int a_column, int a_row);
            void updateBestPlay();
            bool rollDifficultyDice();
            void initialiseWildcardSubstitutes();
            bool isCombinationTriedAlready();
            bool isPositionRelevant(int a_column, int a_row);

            Settings* settings;
            VirtualBoard sandbox_board;
            VirtualBoard best_play_board;
            VirtualBoard hand;
            LetterPool* letter_pool;
            Direction direction;
            int line_index;
            std::vector<std::vector<int>> touch_evaluation;
            std::vector<Letter*> available_letters;
            int available_letter_count = 0;
            int touch_count = 0;
            bool cancelled = false;
            std::random_device random_seed;
            std::mt19937 rng;
            std::uniform_int_distribution<> random_distribution;
            int difficulty;
            std::map<Letter*, std::vector<Letter*>> wildcard_substitutes;
            std::unordered_set<QString> tried_combinations;
            QString current_combination;
            std::stack<int> current_combination_sizes;
    };
}

#endif // __PLAYERAIWORKER_H__