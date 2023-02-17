#ifndef __PLAYERAI_H__
#define __PLAYERAI_H__

#include <QString>
#include <QColor>
#include <QTimer>

#include "ForwardDeclarations.hpp"
#include "Player.hpp"
#include "Settings.hpp"
#include "Tile.hpp"
#include "Board.hpp"

namespace wf
{
    class PlayerAI : public Player
    {
        public:
            PlayerAI(
                QString a_display_name,
                QColor a_color,
                Settings* a_settings,
                Tile* a_selection,
                Board* a_board);
            ~PlayerAI();

        public slots:
            void playIfTurn();

        private:
            void executePlay();

            Board* board;
            QTimer check_turn_timer;
    };
}

#endif // __PLAYERAI_H__