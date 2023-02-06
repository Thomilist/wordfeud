#include "Game.hpp"
namespace wf
{
    Game::Game(
        Settings& a_settings,
        QWidget* a_parent)
        : QWidget(a_parent)
        , settings(a_settings)
        , game_layout(this)
        , header(QSize{
            a_settings.getHandDimensions().width() * a_settings.getHandTileSize().width(),
            a_settings.getHandDimensions().height() * a_settings.getHandTileSize().height()
            },
            this)
        , board(BoardType::Board, a_settings.getGridDimensions(), a_settings.getTileSize(), &selection, this)
        , proposal_info(QSize{
            a_settings.getHandDimensions().width() * a_settings.getHandTileSize().width(),
            a_settings.getHandDimensions().height() * a_settings.getHandTileSize().height()
            },
            this)
        , hands(this)
        , buttons(QSize{
            a_settings.getHandDimensions().width() * a_settings.getHandTileSize().width(),
            a_settings.getHandDimensions().height() * a_settings.getHandTileSize().height() / 2
            },
            this)
        , selection(a_settings.getSelectionTileSize(), &selection, BoardType::None, this, true)
        , rng(std::default_random_engine{})
    {
        initialiseConnections();
        
        setMouseTracking(true);
        hands.setMouseTracking(true);

        createPlayer("Player 1", QColor{128, 0, 0});
        createPlayer("Player 2", QColor{0, 0, 128});

        header.setLeftPlayer(all_players[0]);
        header.setRightPlayer(all_players[1]);
        all_players[current_player_index]->setTurn(true);
        
        int grid_row = 0;

        game_layout.addWidget(&header, grid_row++, 0);
        game_layout.addWidget(&board, grid_row++, 0);
        game_layout.addWidget(&proposal_info, grid_row++, 0);
        game_layout.addWidget(&hands, grid_row++, 0);
        game_layout.addWidget(&buttons, grid_row++, 0);

        setLayout(&game_layout);

        loadLanguage();
        loadLetters();
        letter_pool.set(getAllLetters());

        loadModifiers();
        placeModifiers(getAllModifiers());

        all_players[current_player_index]->fillHand(&letter_pool);

        setCorrectButtonState();
        showCorrectButtons();
    }
    
    Game::~Game()
    {
        for (auto player : all_players)
        {
            delete player;
        }
    }
    
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
    
    void Game::lockProposedLetters()
    {
        for (auto tile : proposed_letters)
        {
            Letter* letter = tile->getLetter();
            letter->setStatus(LetterStatus::LockedRecently);
            locked_letters.push_back(letter);

            Modifier* modifier = tile->getModifier();
            modifier->setUsed(true);
        }

        clearProposed();

        return;
    }
    
    void Game::lockRecentlyLockedLetters()
    {
        for (auto letter : locked_letters)
        {
            if (letter->getStatus() == LetterStatus::LockedRecently)
            {
                letter->setStatus(LetterStatus::Locked);
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
    
    void Game::createPlayer(QString a_display_name, QColor a_color)
    {
        Player* player = new Player{
            a_display_name,
            a_color,
            settings.getHandDimensions(),
            settings.getHandTileSize(),
            &selection,
            this};
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

        all_players[current_player_index]->setTurn(false);

        if (++current_player_index >= all_players.size())
        {
            current_player_index = 0;
        }

        all_players[current_player_index]->fillHand(&letter_pool);
        all_players[current_player_index]->setTurn(true);

        hands.setCurrentIndex(current_player_index);
        repaint();

        return;
    }
    
    void Game::play()
    {
        // Placement check
        if (!isPlacementValid())
        {
            QMessageBox invalid_placement_warning;
            invalid_placement_warning.setText("Invalid placement.");
            invalid_placement_warning.setIcon(QMessageBox::Icon::Warning);
            invalid_placement_warning.setWindowTitle(" ");
            invalid_placement_warning.exec();
            return;
        }

        // Dictionary check
        findProposedWords();
        findInvalidProposedWords();
        
        if (invalid_words.size() > 0)
        {
            QString invalid_words_message{"Word(s) not found in dictionary:"};

            for (const auto word : invalid_words)
            {
                invalid_words_message.append("\n");
                invalid_words_message.append(word->getWordAsText());
            }
            
            QMessageBox invalid_words_warning;
            invalid_words_warning.setText(invalid_words_message);
            invalid_words_warning.setIcon(QMessageBox::Icon::Warning);
            invalid_words_warning.setWindowTitle(" ");
            invalid_words_warning.exec();
            return;
        }

        // Points calculation
        int points = 0;

        for (const auto& word : proposed_words)
        {
            points += word.calculatePoints();
        }

        all_players[current_player_index]->addPoints(points);

        // Find most relevant word
        int highest_match_count = 0;
        Word most_relevant_word;

        for (const auto& word : proposed_words)
        {
            int match_count = word.containsHowManyOf(proposed_letters);

            if (match_count > highest_match_count)
            {
                highest_match_count = match_count;
                most_relevant_word = word;
            }
        }
        
        // Cleanup
        for (const auto tile : proposed_letters)
        {
            tile->getModifier()->setUsed(true);
        }

        lockRecentlyLockedLetters();
        lockProposedLetters();

        // Next turn
        header.updateWithPlay(
            PlayType::Play,
            all_players[current_player_index],
            most_relevant_word.getWordAsText(),
            points);
        nextPlayer();
        
        return;
    }
    
    void Game::pass()
    {
        lockRecentlyLockedLetters();
        header.updateWithPlay(
            PlayType::Pass,
            all_players[current_player_index]);
        nextPlayer();
        return;
    }
    
    void Game::clear()
    {
        for (auto tile : proposed_letters)
        {
            if (all_players[current_player_index]->availableSpacesInHand() > 0)
            {
                Letter* letter = tile->removeLetter();
                all_players[current_player_index]->addLetterToHand(letter);
            }
        }

        clearProposed();
        board.repaint();
        all_players[current_player_index]->getHand()->repaint();

        return;
    }
    
    void Game::shuffle()
    {
        Board* hand = all_players[current_player_index]->getHand();
        QSize hand_size = hand->size();
        std::vector<Tile*> tiles;
        std::vector<Letter*> letters;

        for (int row = 0; row < hand_size.width(); ++row)
        {
            for (int collumn = 0; collumn < hand_size.height(); ++collumn)
            {
                Tile* tile = hand->getTileAtPosition(collumn, row);

                if (tile == nullptr || tile->getLetter() == nullptr)
                {
                    continue;
                }

                tiles.push_back(tile);
                letters.push_back(tile->removeLetter());
            }
        }

        std::shuffle(letters.begin(), letters.end(), rng);

        int index = 0;

        for (auto letter : letters)
        {
            tiles[index++]->placeLetter(letter);
        }
        
        hand->repaint();

        return;
    }
    
    void Game::swap()
    {
        return;
    }
    
    void Game::proposeLetter(Tile* a_tile)
    {
        proposed_letters.push_back(a_tile);
        showCorrectButtons();
        setCorrectButtonState();
        displayProposedPlayValue();
        return;
    }
    
    void Game::unproposeLetter(Tile* a_tile)
    {
        proposed_letters.erase(std::remove(proposed_letters.begin(), proposed_letters.end(), a_tile), proposed_letters.end());
        showCorrectButtons();
        setCorrectButtonState();
        displayProposedPlayValue();
        return;
    }
    
    void Game::clearProposed()
    {
        proposed_letters.clear();
        showCorrectButtons();
        setCorrectButtonState();
        displayProposedPlayValue();
        return;
    }
    
    void Game::setCorrectButtonState()
    {
        bool button_state = selection.getLetter() != nullptr;

        buttons.getPlayButton()->setDisabled(button_state);
        buttons.getPassButton()->setDisabled(button_state);
        buttons.getClearButton()->setDisabled(button_state);
        buttons.getShuffleButton()->setDisabled(button_state);

        bool swap_button_state
            =   button_state 
            ||  (letter_pool.getRemainingCount() < (settings.getHandDimensions().width() * settings.getHandDimensions().height()))
            ||  (proposed_letters.size() != 0);

        buttons.getSwapButton()->setDisabled(swap_button_state);

        return;
    }
    
    void Game::initialiseConnections()
    {
        connect(&letter_pool, &LetterPool::remainingCountChanged, &buttons, &ButtonPanel::setTileCount);
        connect(buttons.getPlayButton(), &QPushButton::clicked, this, &Game::play);
        connect(buttons.getPassButton(), &QPushButton::clicked, this, &Game::pass);
        connect(buttons.getClearButton(), &QPushButton::clicked, this, &Game::clear);
        connect(buttons.getShuffleButton(), &QPushButton::clicked, this, &Game::shuffle);
        connect(buttons.getSwapButton(), &QPushButton::clicked, this, &Game::swap);
        connect(&selection, &Tile::letterAddedRemoved, this, &Game::setCorrectButtonState);

        QSize board_size = board.getGridDimensions();

        for (int collumn = 0; collumn < board_size.width(); ++collumn)
        {
            for (int row = 0; row < board_size.height(); ++row)
            {
                Tile* tile = board.getTileAtPosition(collumn, row);
                connect(tile, &Tile::proposeLetter, this, &Game::proposeLetter);
                connect(tile, &Tile::unproposeLetter, this, &Game::unproposeLetter);
            }
        }

        return;
    }
    
    void Game::mouseMoveEvent(QMouseEvent* a_event)
    {
        selection.move(a_event->pos() + QPoint{1, 1});
        return;
    }
    
    void Game::showCorrectButtons()
    {
        if (proposed_letters.size() > 0)
        {
            buttons.showPlayClearButtons();
        }
        else
        {
            buttons.showPassShuffleButtons();
        }

        return;
    }
    
    bool Game::isPlacementValid()
    {
        checked_tiles.clear();
        return isPlacementConnectedToStart(proposed_letters[0]) && isPlacementLinear();
    }
    
    bool Game::isPlacementConnectedToStart(Tile* a_tile)
    {
        checked_tiles.push_back(a_tile);
        
        if (a_tile->getModifier()->getType() == ModifierType::Start)
        {
            return true;
        }

        std::array<Tile*, 4> neighbours = a_tile->getNeighbours();
        bool start_found = false;

        for (const auto neighbour : neighbours)
        {
            if (neighbour == nullptr)
            {
                continue;
            }
            
            if (std::find(checked_tiles.begin(), checked_tiles.end(), neighbour) != checked_tiles.end())
            {
                continue;
            }
            
            if (neighbour->getLetter() != nullptr)
            {
                start_found = start_found || isPlacementConnectedToStart(neighbour);
            }
        }

        return start_found;
    }
    
    bool Game::isPlacementLinear()
    {
        return isPlacementInOneCollumn() || isPlacementInOneRow();
    }
    
    bool Game::isPlacementInOneCollumn()
    {
        std::vector<int> collumns;
        
        for (const auto tile : proposed_letters)
        {
            collumns.push_back(tile->getGridPosition().x());
        }

        return std::adjacent_find(collumns.begin(), collumns.end(), std::not_equal_to<>()) == collumns.end();
    }
    
    bool Game::isPlacementInOneRow()
    {
        std::vector<int> rows;
        
        for (const auto tile : proposed_letters)
        {
            rows.push_back(tile->getGridPosition().y());
        }

        return std::adjacent_find(rows.begin(), rows.end(), std::not_equal_to<>()) == rows.end();
    }
    
    void Game::findProposedWords()
    {
        proposed_words.clear();
        
        if (proposed_letters.size() == 0)
        {
            return;
        }
        // Single-letter words are allowed only as an opener
        else if (   proposed_letters.size() == 1
                &&  proposed_letters[0]->getModifier()->getType() == ModifierType::Start)
        {
            Word word;
            word.appendLetter(proposed_letters[0]);
            proposed_words.push_back(word);
            return;
        }

        std::unordered_set<int> collumns;
        std::unordered_set<int> rows;

        for (const auto tile : proposed_letters)
        {
            QPoint grid_position = tile->getGridPosition();

            collumns.insert(grid_position.x());
            rows.insert(grid_position.y());
        }

        for (const auto collumn : collumns)
        {
            findProposedWordInLine(collumn, settings.getGridDimensions().height(), Direction::Vertical);
        }

        for (const auto row : rows)
        {
            findProposedWordInLine(row, settings.getGridDimensions().width(), Direction::Horisontal);
        }

        return;
    }
    
    void Game::findProposedWordInLine(int a_line, int a_max_index, Direction a_direction)
    {
        Word word;
            
        for (int index = 0; index < a_max_index; ++index)
        {
            Tile* tile;

            switch (a_direction)
            {
                case Direction::Horisontal:
                {
                    tile = board.getTileAtPosition(index, a_line);
                    break;
                }
                case Direction::Vertical:
                {
                    tile = board.getTileAtPosition(a_line, index);
                    break;
                }
            }

            if (tile->getLetter() == nullptr)
            {
                if (word.containsAnyOf(proposed_letters))
                {
                    if (word.getLength() > 1)
                    {
                        proposed_words.push_back(word);
                    }
                    
                    break;
                }
                else
                {
                    word.clear();
                }
            }
            else
            {
                word.appendLetter(tile);

                if (index == a_max_index - 1)
                {
                    if (    word.containsAnyOf(proposed_letters)
                        &&  word.getLength() > 1)
                    {
                        proposed_words.push_back(word);
                        break;
                    }
                }
            }
        }

        return;
    }
    
    void Game::checkProposedWords()
    {
        // Placement check
        proposed_words_valid = isPlacementValid();

        // Dictionary check
        findProposedWords();
        findInvalidProposedWords();

        if (invalid_words.size() > 0)
        {
            proposed_words_valid = false;
        }

        // Points calculation
        calculateProposedPoints();

        return;
    }
    
    void Game::findInvalidProposedWords()
    {
        invalid_words.clear();

        for (const auto& word : proposed_words)
        {
            if (!language.isInWordList(word.getWordAsText().toLower()))
            {
                invalid_words.push_back(&word);
            }
        }

        return;
    }
    
    void Game::calculateProposedPoints()
    {
        proposed_words_points = 0;

        for (const auto& word : proposed_words)
        {
            proposed_words_points += word.calculatePoints();
        }

        return;
    }
    
    void Game::displayProposedPlayValue()
    {
        checkProposedWords();
        proposal_info.setProposedPlay(proposed_words_valid, proposed_words_points);
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