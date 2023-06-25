#include "Wordfeud.hpp"


namespace wf
{
    Wordfeud::Wordfeud(Version* a_version)
    : settings(&main_window)
    , game(&settings, &main_window)
    , version(a_version)
    , game_menu("Game", &main_window)
    , start_new_game("New game", &game_menu)
    , open_settings("Settings...", &game_menu)
    , settings_dialog(&settings, this, &main_window)
    , show_records("Records...", &game_menu)
    , record_dialog(game.getRecords(), &main_window)
    , about_menu("About", &main_window)
    , check_for_updates("Check for updates...", &about_menu)
    , update_dialog(version, &main_window)
    , open_github("Open on GitHub...", &about_menu)
    {
        connect(&settings_dialog, &SettingsDialog::settingsSaved, &game, &Game::repaintHandAndBoard);
        
        main_window.setCentralWidget(&game);
        main_window.setMouseTracking(true);
        main_window.menuBar()->setMouseTracking(true);
        main_window.setWindowIcon(QIcon{":/icon/icon.ico"});

        initialiseGameMenu();
        initialiseAboutMenu();

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
    
    void Wordfeud::initialiseAboutMenu()
    {
        main_window.menuBar()->addMenu(&about_menu);
        about_menu.addAction(&check_for_updates);
        about_menu.addAction(&open_github);

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