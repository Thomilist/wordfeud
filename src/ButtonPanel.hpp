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

#include "ForwardDeclarations.hpp"

#include "Settings.hpp"

namespace wf
{
    class ButtonPanel : public QWidget
    {
        Q_OBJECT

        public:
            ButtonPanel(Settings* a_settings, QWidget* a_parent = nullptr);
            ~ButtonPanel();

            QPushButton* getPlayButton();
            QPushButton* getPassButton();
            QPushButton* getClearButton();
            QPushButton* getShuffleButton();
            QPushButton* getSwapButton();
            QPushButton* getConfirmButton();
            QPushButton* getCancelButton();
            void showPlayClearSwapButtons();
            void showPassShuffleSwapButtons();
            void showConfirmCancelButtons();

        public slots:
            void setTileCount(int a_count);

        private:
            void drawTileCounter();

            Settings* settings;
            QSize size;
            int button_width;
            QGridLayout button_layout;
            QStackedWidget play_pass_stack;
            QStackedWidget clear_shuffle_confirm_stack;
            QStackedWidget swap_cancel_stack;
            QPushButton play_button;
            QPushButton pass_button;
            QPushButton clear_button;
            QPushButton shuffle_button;
            QPushButton swap_button;
            QPushButton confirm_button;
            QPushButton cancel_button;
            QLabel tiles_left_text;
            QLabel tiles_left_count;
            int tiles_left;

    };
}
#endif // __BUTTONPANEL_H__