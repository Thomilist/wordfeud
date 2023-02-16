#include "Wordfeud.hpp"


namespace wf
{
    Wordfeud::Wordfeud()
    : game(&settings, &main_window)
    , game_menu("Game", &main_window)
    , start_new_game("New game", &game_menu)
    , open_settings("Settings...", &game_menu)
    , settings_dialog(&settings, this, &main_window)
    , about_menu("About", &main_window)
    , open_github("Open on GitHub...", &about_menu)
    {
        main_window.setCentralWidget(&game);

        initialiseGameMenu();
        initialiseAboutMenu();

        main_window.show();
    }
    
    Wordfeud::~Wordfeud()
    { }
    
    void Wordfeud::startNewGame()
    {
        game.reset();
        return;
    }
    
    void Wordfeud::initialiseGameMenu()
    {
        main_window.menuBar()->addMenu(&game_menu);
        game_menu.addAction(&start_new_game);
        game_menu.addAction(&open_settings);

        start_new_game.setShortcut(QKeySequence{"F2"});

        connect(&start_new_game, &QAction::triggered, this, &Wordfeud::startNewGame);
        connect(&open_settings, &QAction::triggered, &settings_dialog, &SettingsDialog::exec);
        connect(settings_dialog.getSaveAndStartNewGameButton(), &QPushButton::clicked, this, &Wordfeud::startNewGame);

        return;
    }
    
    void Wordfeud::initialiseAboutMenu()
    {
        main_window.menuBar()->addMenu(&about_menu);
        about_menu.addAction(&open_github);

        connect(&open_github, &QAction::triggered, this, &Wordfeud::openGitHub);

        return;
    }
    
    void Wordfeud::openGitHub()
    {
        QDesktopServices::openUrl(QUrl{"https://github.com/Thomilist/wordfeud", QUrl::TolerantMode});
        return;
    }
}