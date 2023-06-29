#ifndef __WORDFEUD_H__
#define __WORDFEUD_H__

#include <iostream>
#include <memory>

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

#include "discord/discord.h"

#include "ForwardDeclarations.hpp"

#include "Game.hpp"
#include "RecordDialog.hpp"
#include "Settings.hpp"
#include "SettingsDialog.hpp"
#include "UpdateDialog.hpp"
#include "Version.hpp"

namespace wf
{
    struct DiscordState
    {
        std::unique_ptr<discord::Core> core;
    };
    
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

        private:
            void initialiseGameMenu();
            void initialiseHelpMenu();
            void automaticUpdateCheck();
            void initialiseDiscord();
            void discordResultCallback(discord::Result a_result);
            void discordLogHook(discord::LogLevel a_level, const char* a_message);

            QMainWindow main_window;
            Settings settings;
            Game game;
            Version* version;
            discord::Core* discord_core;
            DiscordState discord_state;

            QMenu game_menu;
            QAction start_new_game;
            QAction open_settings;
            SettingsDialog settings_dialog;
            QAction show_records;
            RecordDialog record_dialog;

            QMenu help_menu;
            QAction view_help;
            QAction check_for_updates;
            UpdateDialog update_dialog;
            QAction open_github;
    };
}


#endif // __WORDFEUD_H__