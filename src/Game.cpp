#include "Game.hpp"
namespace wf
{
    Game::Game(Settings& a_settings, QWidget* a_parent)
        : QWidget(a_parent)
        , settings(a_settings)
        , game_layout(this)
        , header("Header", this)
        , board(a_settings.getGridDimensions(), a_settings.getTileSize(), this)
        , hand("Hand", this)
        , buttons("Buttons", this)
    {
        game_layout.addWidget(&header, 0, 0);
        game_layout.addWidget(&board, 1, 0);
        game_layout.addWidget(&hand, 2, 0);
        game_layout.addWidget(&buttons, 3, 0);

        setLayout(&game_layout);

        loadLanguage();
        setLetters();
        letter_pool.set(getLetters());
    }
    
    Game::~Game()
    { }
    
    void Game::loadLanguage()
    {
        switch (settings.getLanguage())
        {
            case LanguageName::English:
            {
                language.loadWordListFromFile(":/word-lists/english.txt");
                language.loadLettersFromFile(":/letter-sets/english.csv");
                break;
            }
            case LanguageName::Danish:
            {

                break;
            }
        }
    }
    
    void Game::setLetters()
    {
        all_letters.clear();

        for (const auto& letter_data : language.getLetterList())
        {
            for (int n = 0; n < letter_data.count; ++n)
            {
                Letter letter{letter_data.letter, letter_data.points};
                all_letters.push_back(letter);
            }
        }

        return;
    }

    std::vector<Letter*> Game::getLetters()
    {
        std::vector<Letter*> letter_pointers;

        for (auto& letter : all_letters)
        {
            letter_pointers.push_back(&letter);
        }

        return letter_pointers;
    }
}