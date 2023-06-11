#ifndef __PLAYERAI_H__
#define __PLAYERAI_H__

#include <vector>

#include <QChar>
#include <QColor>
#include <QPoint>
#include <QString>
#include <QThread>
#include <QTimer>

#include "ForwardDeclarations.hpp"

#include "Letter.hpp"
#include "Player.hpp"
#include "RenderedTile.hpp"
#include "Settings.hpp"
#include "VirtualBoard.hpp"
#include "VirtualTile.hpp"

#include "Direction.hpp"
#include "LetterType.hpp"

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
        public:
            PlayerAI(
                QString a_display_name,
                QColor a_color,
                Settings* a_settings,
                RenderedTile* a_selection,
                RenderedBoard* a_board);
            ~PlayerAI();

            virtual void setTurn(bool a_has_turn);

        public slots:
            void playIfTurn();

        private:
            void executeTurn();
            void startOfTurnSetup();
            void fetchAvailableLetters();
            void findBestPlay();
            void findPlayInLine(Direction a_direction, int a_index);
            void findPlayInHorisontalLine(int a_row);
            void findPlayInVerticalLine(int a_collumn);
            void findPlayRecursively(int a_collumn, int a_row, Direction a_direction);
            void tryLetterAndRecurse(Letter*& a_letter, VirtualTile* a_tile, int a_collumn, int a_row, Direction a_direction);
            void recurse(int a_collumn, int a_row, Direction a_direction);
            bool indexOutOfBounds(int a_collumn, int a_row);
            void updateBestPlay();
            void executeBestPlay();
            void setBestPlayWildcardLetters(std::vector<VirtualTile*> a_tiles);

            RenderedBoard* live_board;
            VirtualBoard sandbox_board;
            VirtualBoard best_play;
            std::vector<QChar> best_play_wildcard_letters;
            QTimer check_turn_timer;
            std::vector<Letter*> available_letters;
            int available_letter_count;
    };
}

#endif // __PLAYERAI_H__