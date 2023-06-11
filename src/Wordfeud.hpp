#ifndef __WORDFEUD_H__
#define __WORDFEUD_H__

#include <QAction>
#include <QDesktopServices>
#include <QIcon>
#include <QKeySequence>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QUrl>

#include "ForwardDeclarations.hpp"

#include "Game.hpp"
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

            QMainWindow main_window;
            Settings settings;
            Game game;

            QMenu game_menu;
            QAction start_new_game;
            QAction open_settings;
            SettingsDialog settings_dialog;

            QMenu about_menu;
            QAction open_github;
    };
}


#endif // __WORDFEUD_H__