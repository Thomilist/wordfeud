#ifndef __PLAYERAI_H__
#define __PLAYERAI_H__

#include <unordered_set>

#include <QString>
#include <QChar>
#include <QColor>
#include <QTimer>

#include "ForwardDeclarations.hpp"

#include "Player.hpp"
#include "Settings.hpp"
#include "Tile.hpp"
#include "Board.hpp"
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
                Tile* a_selection,
                Board* a_board);
            ~PlayerAI();

        public slots:
            void playIfTurn();

        private:
            void executePlay();
            void makeReducedWordList();

            Board* board;
            VirtualBoard virtual_board;
            QTimer check_turn_timer;
            int longest_possible_word;
            std::unordered_set<QString> word_list;
            std::unordered_set<QString> reduced_word_list;
    };
}

#endif // __PLAYERAI_H__