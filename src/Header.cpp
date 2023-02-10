#include "Header.hpp"

namespace wf
{
    Header::Header(
        Settings* a_settings,
        QWidget* a_parent)
        : QWidget(a_parent)
        , settings(a_settings)
    {
        size = QSize{
            settings->getHandDimensions().width() * settings->getHandTileSize().width(),
            settings->getHandDimensions().height() * settings->getHandTileSize().height()
        };
        
        setFixedWidth(size.width());
        setMouseTracking(true);

        left_player_name.setFixedHeight(size.height() / 4);
        right_player_name.setFixedHeight(size.height() / 4);

        grid_layout.addWidget(&left_player_name, 0, 0);
        grid_layout.addWidget(&right_player_name, 0, 1);
        grid_layout.addWidget(&left_player_score, 1, 0);
        grid_layout.addWidget(&right_player_score, 1, 1);
        grid_layout.addWidget(&left_player_your_turn, 2, 0);
        grid_layout.addWidget(&right_player_your_turn, 2, 1);
        grid_layout.addWidget(&last_play_text, 3, 0, 1, 2);

        right_player_name.setAlignment(Qt::AlignRight);
        right_player_score.setAlignment(Qt::AlignRight);
        right_player_your_turn.setAlignment(Qt::AlignRight);
        last_play_text.setAlignment(Qt::AlignCenter);

        QFont score_font = settings->getMonospaceFont();
        score_font.setPointSize(size.height() / 5);
        score_font.setBold(true);

        left_player_score.setFont(score_font);
        right_player_score.setFont(score_font);

        grid_layout.setSpacing(0);
        setLayout(&grid_layout);
    }
    
    Header::~Header()
    { }
    
    void Header::setLeftPlayer(Player* a_player)
    {
        left_player = a_player;
        return;
    }
    
    void Header::setRightPlayer(Player* a_player)
    {
        right_player = a_player;
        return;
    }
    
    void Header::updateWithPlay(PlayType a_play_type, Player* a_player, QString a_word, int a_value)
    {
        last_play_type = a_play_type;
        last_play_player = a_player;
        last_play_word = a_word;
        last_play_value = a_value;
        repaint();
        return;
    }
    
    void Header::paintEvent(QPaintEvent*)
    {
        QFont name_font = settings->getMonospaceFont();
        name_font.setPointSize(size.height() / 8);

        if (left_player != nullptr)
        {
            QPixmap pixmap{left_player_name.size()};
            QPainter painter(&pixmap);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setFont(name_font);

            painter.setPen(Qt::NoPen);
            painter.setBrush(left_player->getColor());
            painter.drawRect(pixmap.rect());

            painter.setPen(QColor{255, 255, 255});
            painter.drawText(pixmap.rect(), left_player->getDisplayName());

            left_player_name.setPixmap(pixmap);
            left_player_score.setText(left_player->getScoreAsText());

            if (left_player->hasTurn())
            {
                left_player_your_turn.setText("Your turn");
            }
            else
            {
                left_player_your_turn.setText(" ");
            }
        }

        if (right_player != nullptr)
        {
            QPixmap pixmap{right_player_name.size()};
            QPainter painter(&pixmap);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setFont(name_font);

            painter.setPen(Qt::NoPen);
            painter.setBrush(right_player->getColor());
            painter.drawRect(pixmap.rect());

            painter.setPen(QColor{255, 255, 255});
            painter.drawText(pixmap.rect(), right_player->getDisplayName(), QTextOption{Qt::AlignRight});

            right_player_name.setPixmap(pixmap);
            right_player_score.setText(right_player->getScoreAsText());

            if (right_player->hasTurn())
            {
                right_player_your_turn.setText("Your turn");
            }
            else
            {
                right_player_your_turn.setText(" ");
            }
        }

        if (last_play_player != nullptr)
        {
            QString last_play;

            switch (last_play_type)
            {
                case PlayType::None:
                {
                    last_play = " ";
                    break;
                }
                case PlayType::Play:
                {
                    last_play.append(last_play_player->getDisplayName());
                    last_play.append(" played ");
                    last_play.append(last_play_word);
                    last_play.append(" for ");
                    last_play.append(QString::number(last_play_value));
                    last_play.append(" points");
                    break;
                }
                case PlayType::Pass:
                {
                    last_play.append(last_play_player->getDisplayName());
                    last_play.append(" passed their turn");
                    break;
                }
                case PlayType::Swap:
                {
                    last_play.append(last_play_player->getDisplayName());
                    last_play.append(" swapped ");
                    last_play.append(QString::number(last_play_value));
                    last_play.append(" tiles");
                    break;
                }
            }

            last_play_text.setText(last_play);
        }
        else
        {
            last_play_text.setText(" ");
        }

        return;
    }
}