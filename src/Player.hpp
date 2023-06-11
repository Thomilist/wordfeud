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

#include "PlayerType.hpp"

namespace wf
{
    class Player : public QObject
    {
        Q_OBJECT
        
        public:
            Player(
                QString a_display_name,
                PlayerType a_type,
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
            virtual void setTurn(bool a_has_turn);
            bool hasTurn();
            void setColor(QColor a_color);
            QColor getColor();
            int getLetterPenaltyPoints();
            void setType(PlayerType a_type);
            PlayerType getType();
        
        protected:
            Settings* settings;
            bool has_turn = false;

        private:
            QString display_name;
            PlayerType type;
            QColor name_color;
            int score = 0;
            QWidget hand_widget;
            QGridLayout hand_layout;
            RenderedBoard hand;
            QSize hand_size;
    };
}

#endif // __PLAYER_H__