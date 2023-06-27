#include "Wordfeud.hpp"


namespace wf
{
    Wordfeud::Wordfeud(Version* a_version)
    : settings(&main_window)
    , game(&settings, &main_window)
    , version(a_version)
    , game_menu("Game", &main_window)
    , start_new_game("New game", &game_menu)
    , open_settings("Settings", &game_menu)
    , settings_dialog(&settings, this, &main_window)
    , show_records("Records", &game_menu)
    , record_dialog(game.getRecords(), &main_window)
    , help_menu("Help", &main_window)
    , view_help("View help", &help_menu)
    , check_for_updates("Check for updates", &help_menu)
    , update_dialog(version, &main_window)
    , open_github("Open on GitHub", &help_menu)
    {
        connect(&settings_dialog, &SettingsDialog::settingsSaved, &game, &Game::repaintHandAndBoard);
        
        main_window.setCentralWidget(&game);
        main_window.setMouseTracking(true);
        main_window.menuBar()->setMouseTracking(true);
        main_window.setWindowIcon(QIcon{":/icon/icon.ico"});

        initialiseGameMenu();
        initialiseHelpMenu();

        main_window.show();
        automaticUpdateCheck();
    }
    
    Wordfeud::~Wordfeud()
    { }
    
    void Wordfeud::closeWindow()
    {
        game.terminatePlayerAI();
        return;
    }
    
    void Wordfeud::startNewGame()
    {
        game.reset();
        return;
    }
    
    void Wordfeud::viewHelp()
    {
        QDesktopServices::openUrl(QUrl{"https://github.com/Thomilist/wordfeud/wiki/How-to-play/", QUrl::TolerantMode});
        return;
    }
    
    void Wordfeud::initialiseGameMenu()
    {
        main_window.menuBar()->addMenu(&game_menu);
        game_menu.addAction(&start_new_game);
        game_menu.addAction(&show_records);
        game_menu.addAction(&open_settings);

        start_new_game.setShortcut(QKeySequence{"F2"});
        show_records.setShortcut(QKeySequence{"F3"});
        open_settings.setShortcut(QKeySequence{"Escape"});

        connect(&start_new_game, &QAction::triggered, this, &Wordfeud::startNewGame);
        connect(&show_records, &QAction::triggered, &record_dialog, &RecordDialog::open);
        connect(&open_settings, &QAction::triggered, &settings_dialog, &SettingsDialog::open);
        connect(settings_dialog.getSaveAndStartNewGameButton(), &QPushButton::clicked, this, &Wordfeud::startNewGame);

        return;
    }
    
    void Wordfeud::initialiseHelpMenu()
    {
        main_window.menuBar()->addMenu(&help_menu);
        help_menu.addAction(&view_help);
        help_menu.addAction(&check_for_updates);
        help_menu.addAction(&open_github);

        view_help.setShortcut(QKeySequence{"F1"});

        connect(&view_help, &QAction::triggered, this, &Wordfeud::viewHelp);
        connect(&check_for_updates, &QAction::triggered, &update_dialog, &UpdateDialog::manualUpdateCheck);
        connect(&open_github, &QAction::triggered, this, &Wordfeud::openGitHub);

        return;
    }
    
    void Wordfeud::automaticUpdateCheck()
    {
        if (version->checkedRecently())
        {
            return;
        }

        update_dialog.open(version->isUpdateAvailable(), true);
        return;
    }
    
    void Wordfeud::openGitHub()
    {
        QDesktopServices::openUrl(QUrl{"https://github.com/Thomilist/wordfeud", QUrl::TolerantMode});
        return;
    }
}