#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <QWidget>
#include <QSize>

#include "ForwardDeclarations.hpp"
#include "Board.hpp"
#include "LetterPool.hpp"

namespace wf
{
    class Player
    {
        public:
            Player(const QSize& a_hand_size, const QSize& a_tile_size, Tile* a_selection, QWidget* a_parent = nullptr);
            ~Player();

            Board* getHand();
            void fillHand(LetterPool* a_letter_pool);
        
        private:
            Board hand;
            QSize hand_size;
    };
}

#endif // __PLAYER_H__