#ifndef __PLAYERAI_H__
#define __PLAYERAI_H__

#include <QChar>
#include <QColor>
#include <QString>
#include <QTimer>

#include "ForwardDeclarations.hpp"

#include "Player.hpp"
#include "RenderedTile.hpp"
#include "Settings.hpp"
#include "VirtualBoard.hpp"
#include "VirtualTile.hpp"

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
                VirtualBoard* a_board);
            ~PlayerAI();

        public slots:
            void playIfTurn();

        private:
            void executePlay();

            VirtualBoard board;
            QTimer check_turn_timer;
    };
}

#endif // __PLAYERAI_H__