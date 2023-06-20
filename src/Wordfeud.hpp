#ifndef __WORDFEUD_H__
#define __WORDFEUD_H__

#include <QAction>
#include <QDesktopServices>
#include <QEventLoop>
#include <QIcon>
#include <QKeySequence>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QThread>
#include <QUrl>

#include "ForwardDeclarations.hpp"

#include "Game.hpp"
#include "RecordDialog.hpp"
#include "Settings.hpp"
#include "SettingsDialog.hpp"

namespace wf
{
    class Wordfeud : public QObject
    {
        Q_OBJECT
        
        public:
            Wordfeud();
            ~Wordfeud();

        public slots:
            void closeWindow();
            void startNewGame();
            void openGitHub();

        private:
            void initialiseGameMenu();
            void initialiseAboutMenu();

            QEventLoop wait_for_close;
            QTimer close_timer;
            int close_timeout = 20000;

            QMainWindow main_window;
            Settings settings;
            Game game;

            QMenu game_menu;
            QAction start_new_game;
            QAction open_settings;
            QAction show_records;
            SettingsDialog settings_dialog;
            RecordDialog record_dialog;

            QMenu about_menu;
            QAction open_github;
    };
}


#endif // __WORDFEUD_H__