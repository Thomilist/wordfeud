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
#include <QUrl>

#include "ForwardDeclarations.hpp"

#include "Game.hpp"
#include "RecordDialog.hpp"
#include "Settings.hpp"
#include "SettingsDialog.hpp"
#include "UpdateDialog.hpp"
#include "Version.hpp"

namespace wf
{
    class Wordfeud : public QObject
    {
        Q_OBJECT
        
        public:
            Wordfeud(Version* a_version);
            ~Wordfeud();

        public slots:
            void closeWindow();
            void startNewGame();
            void openGitHub();

        private:
            void initialiseGameMenu();
            void initialiseAboutMenu();
            void automaticUpdateCheck();

            QMainWindow main_window;
            Settings settings;
            Game game;
            Version* version;

            QMenu game_menu;
            QAction start_new_game;
            QAction open_settings;
            SettingsDialog settings_dialog;
            QAction show_records;
            RecordDialog record_dialog;

            QMenu about_menu;
            QAction check_for_updates;
            UpdateDialog update_dialog;
            QAction open_github;
    };
}


#endif // __WORDFEUD_H__