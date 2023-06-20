#ifndef __PLAYERAIWORKER_H__
#define __PLAYERAIWORKER_H__

#include <set>
#include <vector>

#include <QChar>
#include <QObject>
#include <QPoint>

#include "Letter.hpp"
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
                Direction a_direction,
                int a_line_index,
                std::set<int> a_relevant_rows,
                std::set<int> a_relevant_columns,
                std::vector<std::vector<int>> a_touch_evaluation);
            ~PlayerAIWorker();

            int getScore();
            std::vector<QPoint> getLetterPositionsInHand();
            std::vector<QPoint> getLetterPositionsOnBoard();
            std::vector<QChar> getWildcardLetters();

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
            void findPlayRecursively(int a_column, int a_row);
            void tryLetterAndRecurse(Letter*& a_letter, VirtualTile* a_tile, int a_column, int a_row);
            void recurse(int a_column, int a_row);
            bool indexOutOfBounds(int a_column, int a_row);
            void updateBestPlay();
            void setBestPlayWildcardLetters();

            Settings* settings;
            VirtualBoard sandbox_board;
            VirtualBoard best_play_board;
            VirtualBoard hand;
            Direction direction;
            int line_index;
            std::set<int> relevant_rows;
            std::set<int> relevant_columns;
            std::vector<std::vector<int>> touch_evaluation;
            std::vector<Letter*> available_letters;
            int available_letter_count = 0;
            int touch_count = 0;
            std::vector<QChar> best_play_wildcard_letters;
            bool cancelled = false;
    };
}

#endif // __PLAYERAIWORKER_H__