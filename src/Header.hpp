#ifndef __HEADER_H__
#define __HEADER_H__

#include <memory>

#include <QWidget>
#include <QGridLayout>
#include <QSize>
#include <QLabel>
#include <QFont>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QTextOption>

#include "ForwardDeclarations.hpp"

#include "Player.hpp"
#include "Settings.hpp"

#include "PlayType.hpp"

namespace wf
{
    class Header : public QWidget
    {
        public:
            Header(
                Settings* a_settings,
                QWidget* a_parent = nullptr);
            ~Header();

            void setLeftPlayer(Player* a_player);
            void setRightPlayer(Player* a_player);
            void updateWithPlay(PlayType a_play_type, Player* a_player = nullptr, QString a_word = "", int a_value = 0);
            void reset();
        
        private:
            void paintEvent(QPaintEvent* a_event);

            Settings* settings;
            QSize size;
            Player* left_player = nullptr;
            Player* right_player = nullptr;
            QGridLayout grid_layout;
            QLabel left_player_name;
            QLabel left_player_score;
            QLabel left_player_your_turn;
            QLabel right_player_name;
            QLabel right_player_score;
            QLabel right_player_your_turn;
            PlayType last_play_type = PlayType::None;
            QLabel last_play_text;
            Player* last_play_player = nullptr;
            QString last_play_word;
            int last_play_value;
    };
}

#endif // __HEADER_H__