#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <QWidget>
#include <QSize>
#include <QString>
#include <QColor>

#include "ForwardDeclarations.hpp"
#include "Board.hpp"
#include "LetterPool.hpp"
#include "Letter.hpp"

namespace wf
{
    class Player
    {
        public:
            Player(
                QString a_display_name,
                QColor a_color,
                const QSize& a_hand_size,
                const QSize& a_tile_size,
                Tile* a_selection,
                QWidget* a_parent = nullptr);
            ~Player();

            Board* getHand() const;
            void fillHand(LetterPool* a_letter_pool);
            int availableSpacesInHand();
            void addLetterToHand(Letter* a_letter);
            void addPoints(int a_points);
            int getScore();
            QString getScoreAsText() const;
            QString getDisplayName() const;
            void setTurn(bool a_has_turn);
            bool hasTurn();
            void setColor(QColor a_color);
            QColor getColor();
        
        private:
            QString display_name;
            QColor name_color;
            int score = 0;
            Board* hand;
            QSize hand_size;
            bool has_turn = false;
    };
}

#endif // __PLAYER_H__