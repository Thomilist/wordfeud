#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <QColor>
#include <QGridLayout>
#include <QObject>
#include <QSize>
#include <QString>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "Letter.hpp"
#include "LetterPool.hpp"
#include "RenderedBoard.hpp"
#include "RenderedTile.hpp"
#include "Settings.hpp"

namespace wf
{
    class Player : public QObject
    {
        Q_OBJECT
        
        public:
            Player(
                QString a_display_name,
                QColor a_color,
                Settings* a_settings,
                RenderedTile* a_selection);
            ~Player();

            RenderedBoard* getHand();
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
        
        protected:
            Settings* settings;

        private:
            QString display_name;
            QColor name_color;
            int score = 0;
            QWidget hand_widget;
            QGridLayout hand_layout;
            RenderedBoard hand;
            QSize hand_size;
            bool has_turn = false;
    };
}

#endif // __PLAYER_H__