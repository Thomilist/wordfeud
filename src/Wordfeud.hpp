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
#include <QMessageBox>
#include <QObject>
#include <QUrl>

#include "ForwardDeclarations.hpp"

#include "AboutDialog.hpp"
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
            void viewHelp();
            void openGitHub();
            void reportIssue();

        private:
            void initialiseGameMenu();
            void initialiseHelpMenu();
            void automaticUpdateCheck();

            QMainWindow main_window;
            Settings settings;
            Version* version;
            Game game;

            QMenu game_menu;
            QAction start_new_game;
            QAction open_settings;
            SettingsDialog settings_dialog;
            QAction show_records;
            RecordDialog record_dialog;

            QMenu help_menu;
            QAction view_help;
            QAction view_about;
            AboutDialog about_dialog;
            QAction check_for_updates;
            UpdateDialog update_dialog;
            QAction open_github;
            QAction report_issue;
    };
}


#endif // __WORDFEUD_H__