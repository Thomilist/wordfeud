#ifndef __BUTTONPANEL_H__
#define __BUTTONPANEL_H__

#include <vector>

#include <QWidget>
#include <QGridLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QSizePolicy>
#include <QSize>
#include <QFont>
#include <QPainter>
#include <QString>

namespace wf
{
    class ButtonPanel : public QWidget
    {
        Q_OBJECT

        public:
            ButtonPanel(QSize a_size, QWidget* a_parent = nullptr);
            ~ButtonPanel();

            QPushButton* getPlayButton();
            QPushButton* getPassButton();
            QPushButton* getClearButton();
            QPushButton* getShuffleButton();
            QPushButton* getSwapButton();
            void showPlayClearButtons();
            void showPassShuffleButtons();

        public slots:
            void setTileCount(int a_count);

        private:
            void drawTileCounter();

            QSize size;
            int button_width;
            QGridLayout button_layout;
            QStackedWidget play_pass_buttons;
            QStackedWidget clear_shuffle_buttons;
            QPushButton play_button;
            QPushButton pass_button;
            QPushButton clear_button;
            QPushButton shuffle_button;
            QPushButton swap_button;
            QLabel tiles_left_text;
            QLabel tiles_left_count;
            int tiles_left;

    };
}
#endif // __BUTTONPANEL_H__