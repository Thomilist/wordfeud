#include "Game.hpp"
namespace wf
{
    Game::Game(Settings& a_settings, QWidget* a_parent)
        : QWidget(a_parent)
        , settings(a_settings)
        , game_layout(this)
        , header("Header", this)
        , board(BoardType::Board, a_settings.getGridDimensions(), a_settings.getTileSize(), &selection, this)
        , hands(this)
        , buttons("Buttons", this)
        , selection(a_settings.getSelectionTileSize(), &selection, BoardType::None, this, true)
    {
        setMouseTracking(true);
        header.setMouseTracking(true);
        hands.setMouseTracking(true);
        buttons.setMouseTracking(true);

        createPlayer("Player 1");
        createPlayer("Player 2");
        
        game_layout.addWidget(&header, 0, 0);
        game_layout.addWidget(&board, 1, 0);
        game_layout.addWidget(&hands, 2, 0);
        game_layout.addWidget(&buttons, 3, 0);

        setLayout(&game_layout);

        loadLanguage();
        loadLetters();
        letter_pool.set(getAllLetters());

        loadModifiers();
        placeModifiers(getAllModifiers());

        all_players[current_player_index]->fillHand(&letter_pool);
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
    
    void Game::loadLetters()
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
    
    void Game::placeLetter(int a_collumn, int a_row, Letter* a_letter)
    {
        Tile* tile = board.getTileAtPosition(a_collumn, a_row);

        if (tile == nullptr)
        {
            return;
        }

        tile->placeLetter(a_letter);

        return;
    }
    
    Letter* Game::removeLetter(int a_collumn, int a_row)
    {
        Tile* tile = board.getTileAtPosition(a_collumn, a_row);

        if (tile == nullptr)
        {
            return nullptr;
        }

        return tile->removeLetter();
    }
    
    void Game::lockLetters()
    {
        QSize grid_dimensions = board.getGridDimensions();

        for (int collumn = 0; collumn < grid_dimensions.width(); ++collumn)
        {
            for (int row = 0; row < grid_dimensions.height(); ++row)
            {
                Tile* tile = board.getTileAtPosition(collumn, row);

                if (tile == nullptr)
                {
                    continue;
                }

                if (tile->getLetter()->getStatus() == LetterStatus::LockedRecently)
                {
                    tile->getLetter()->setStatus(LetterStatus::Locked);
                }
                else
                {
                    tile->getLetter()->setStatus(LetterStatus::LockedRecently);
                }
                
            }
        }

        return;
    }
    
    void Game::loadModifiers()
    {
        all_modifiers = {
            ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter,
            ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None,
            ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None,
            ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None,
            ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleWord,
            ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None,
            ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None,
            ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::Start, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter,
            ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None,
            ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None,
            ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleWord,
            ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, 
            ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None,
            ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None,
            ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter
        };
    }
    
    void Game::placeModifier(int a_collumn, int a_row, Modifier* a_modifier, bool a_overwrite)
    {
        Tile* tile = board.getTileAtPosition(a_collumn, a_row);

        if (tile == nullptr)
        {
            return;
        }

        Modifier* current_modifier = tile->getModifier();

        if (!a_overwrite && current_modifier != nullptr && current_modifier->getType() != ModifierType::None)
        {
            return;
        }

        if (current_modifier != nullptr && current_modifier->getType() == ModifierType::Start)
        {
            return;
        }

        tile->setModifier(a_modifier);

        return;
    }
    
    void Game::placeModifiers(std::vector<Modifier*> a_modifiers)
    {
        QSize grid_dimensions = board.getGridDimensions();
        long unsigned int modifier_index = 0;

        for (int row = 0; row < grid_dimensions.height(); ++row)
        {
            for (int collumn = 0; collumn < grid_dimensions.width(); ++collumn)
            {
                Tile* tile = board.getTileAtPosition(collumn, row);

                if (tile == nullptr)
                {
                    continue;
                }

                if (modifier_index < a_modifiers.size())
                {
                    tile->setModifier(a_modifiers[modifier_index]);
                    ++modifier_index;
                }
            }
        }

        return;
    }
    
    void Game::createPlayer(QString a_display_name)
    {
        Player* player = new Player{a_display_name, settings.getHandDimensions(), settings.getHandTileSize(), &selection, this};
        all_players.push_back(player);
        hands.addWidget(player->getHand());
        return;
    }
    
    void Game::nextPlayer()
    {
        if (all_players.size() == 0)
        {
            return;
        }

        if (++current_player_index > all_players.size())
        {
            current_player_index = 0;
        }

        hands.setCurrentIndex(current_player_index);

        return;
    }
    
    void Game::mouseMoveEvent(QMouseEvent* a_event)
    {
        selection.move(a_event->pos() + QPoint{1, 1});
        return;
    }

    std::vector<Letter*> Game::getAllLetters()
    {
        std::vector<Letter*> letter_pointers;

        for (auto& letter : all_letters)
        {
            letter_pointers.push_back(&letter);
        }

        return letter_pointers;
    }

    std::vector<Modifier*> Game::getAllModifiers()
    {
        std::vector<Modifier*> modifier_pointers;

        for (auto& modifier : all_modifiers)
        {
            modifier_pointers.push_back(&modifier);
        }

        return modifier_pointers;
    }
}