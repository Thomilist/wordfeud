#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <QWidget>
#include <QSize>
#include <QString>
#include <QColor>
#include <QGridLayout>

#include "ForwardDeclarations.hpp"
#include "Board.hpp"
#include "LetterPool.hpp"
#include "Letter.hpp"
#include "Settings.hpp"

namespace wf
{
    class Player
    {
        public:
            Player(
                QString a_display_name,
                QColor a_color,
                Settings* a_settings,
                Tile* a_selection);
            ~Player();

            Board* getHand();
            QWidget* getHandCentered();
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
            int getLetterPenaltyPoints();
        
        private:
            QString display_name;
            QColor name_color;
            Settings* settings;
            int score = 0;
            QWidget hand_widget;
            QGridLayout hand_layout;
            Board hand;
            QSize hand_size;
            bool has_turn = false;
    };
}

#endif // __PLAYER_H__