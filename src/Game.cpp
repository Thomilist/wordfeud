#include "Game.hpp"
namespace wf
{
    Game::Game(
        Settings* a_settings,
        QWidget* a_parent)
        : QWidget(a_parent)
        , settings(a_settings)
        , game_layout(this)
        , selection(a_settings, &selection, BoardType::Selection, this, true)
        , header(a_settings, this)
        , board(BoardType::Board, a_settings, &selection, this)
        , proposal_info(a_settings, this)
        , hands(this)
        , buttons(a_settings, this)
        , rng(std::default_random_engine{})
    {
        setMouseTracking(true);
        hands.setMouseTracking(true);

        createPlayers();

        header.setLeftPlayer(all_players[0]);
        header.setRightPlayer(all_players[1]);
        all_players[current_player_index]->setTurn(true);

        header.stackUnder(&selection);
        board.stackUnder(&selection);
        proposal_info.stackUnder(&selection);
        hands.stackUnder(&selection);
        buttons.stackUnder(&selection);
        
        int grid_row = 0;

        game_layout.addWidget(&header, grid_row++, 0);
        game_layout.addWidget(&board, grid_row++, 0);
        game_layout.addWidget(&proposal_info, grid_row++, 0);
        game_layout.addWidget(&hands, grid_row++, 0);
        game_layout.addWidget(&buttons, grid_row++, 0);

        setLayout(&game_layout);

        loadLetters();
        letter_pool.set(getAllLetters());

        placeModifiers(settings->getModifierPattern()->getModifiers());

        for (auto& player : all_players)
        {
            player->fillHand(&letter_pool);
        }

        buttons.setTileCount(letter_pool.getRemainingCount());
        setCorrectButtonState();
        showCorrectButtons();

        initialiseConnections();
    }
    
    Game::~Game()
    {
        deletePlayers();
    }
    
    void Game::loadLetters()
    {
        all_letters.clear();

        for (const auto& letter_data : settings->getLanguage()->getLetterList())
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
        auto player = new Player
        (
            a_display_name,
            a_color,
            settings,
            &selection
        );

        hands.addWidget(player->getHand());
        all_players.push_back(player);
        return;
    }
    
    void Game::createPlayers()
    {
        createPlayer("Player 1", QColor{128, 0, 0});
        createPlayer("Player 2", QColor{0, 0, 128});
        return;
    }
    
    void Game::deletePlayers()
    {
        for (auto player : all_players)
        {
            delete player;
        }

        all_players.clear();
        return;
    }
    
    void Game::nextPlayer()
    {
        if (all_players.size() == 0)
        {
            return;
        }

        if (isGameOver())
        {
            finalisePoints();
            Player* winning_player = getHighestScoringPlayer();
            QMessageBox game_over;

            if (winning_player != nullptr)
            {
                game_over.setText(winning_player->getDisplayName() + " wins!");
            }
            else
            {
                game_over.setText("Draw.");
            }
            
            game_over.setIcon(QMessageBox::Icon::Information);
            game_over.setWindowTitle(" ");
            game_over.exec();
        }
        else
        {
            all_players[current_player_index]->setTurn(false);

            if (++current_player_index >= all_players.size())
            {
                current_player_index = 0;
            }

            all_players[current_player_index]->fillHand(&letter_pool);
            all_players[current_player_index]->setTurn(true);

            hands.setCurrentIndex(current_player_index);
        }

        repaint();
        return;
    }
    
    void Game::reset()
    {
        state = GameState::Play;
        current_player_index = 0;
        proposed_words_points = 0;
        proposed_words_valid = true;
        consecutive_passes = 0;

        all_letters.clear();
        proposed_letters.clear();
        locked_letters.clear();
        swap_letters.clear();
        checked_tiles.clear();
        proposed_words.clear();
        invalid_words.clear();

        deletePlayers();

        header.reset();
        board.reset();
        proposal_info.setProposedPlay(true, 0);

        createPlayers();

        header.setLeftPlayer(all_players[0]);
        header.setRightPlayer(all_players[1]);
        all_players[current_player_index]->setTurn(true);

        loadLetters();
        letter_pool.set(getAllLetters());

        placeModifiers(settings->getModifierPattern()->getModifiers());

        for (auto& player : all_players)
        {
            player->fillHand(&letter_pool);
        }

        buttons.setTileCount(letter_pool.getRemainingCount());
        setCorrectButtonState();
        showCorrectButtons();

        repaint();

        return;
    }
    
    void Game::playButton()
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

        // 40 point bonus for using all letters in hand
        if (proposed_letters.size() == static_cast<long unsigned int>(all_players[current_player_index]->getHand()->getTileCount()))
        {
            points += 40;
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
        consecutive_passes = 0;
        header.updateWithPlay(
            PlayType::Play,
            all_players[current_player_index],
            most_relevant_word.getWordAsText(),
            points);
        nextPlayer();
        
        return;
    }
    
    void Game::passButton()
    {
        lockRecentlyLockedLetters();
        ++consecutive_passes;
        header.updateWithPlay(
            PlayType::Pass,
            all_players[current_player_index]);
        nextPlayer();
        return;
    }
    
    void Game::clearButton()
    {
        Letter* letter;
        Tile* tile;

        while (proposed_letters.size() > 0)
        {
            tile = proposed_letters.at(0);
            
            if (all_players[current_player_index]->availableSpacesInHand() > 0)
            {
                letter = tile->removeLetter();
                all_players[current_player_index]->addLetterToHand(letter);
            }
            else
            {
                break;
            }
        }

        clearProposed();
        board.repaint();
        all_players[current_player_index]->getHand()->repaint();

        return;
    }
    
    void Game::shuffleButton()
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
    
    void Game::swapButton()
    {
        setGameState(GameState::Swap);
        showCorrectButtons();
        return;
    }
    
    void Game::confirmButton()
    {
        int swap_count = swapLetters();
        clearSwapList();
        setGameState(GameState::Play);
        showCorrectButtons();

        // Next turn
        consecutive_passes = 0;
        header.updateWithPlay(
            PlayType::Swap,
            all_players[current_player_index],
            "",
            swap_count);
        nextPlayer();

        return;
    }
    
    void Game::cancelButton()
    {
        clearSwapList();
        setGameState(GameState::Play);
        showCorrectButtons();
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
            ||  (letter_pool.getRemainingCount() < (all_players[current_player_index]->getHand()->getTileCount()))
            ||  (proposed_letters.size() != 0);

        buttons.getSwapButton()->setDisabled(swap_button_state);

        return;
    }
    
    void Game::initialiseConnections()
    {
        connect(&letter_pool, &LetterPool::remainingCountChanged, &buttons, &ButtonPanel::setTileCount);
        connect(buttons.getPlayButton(), &QPushButton::clicked, this, &Game::playButton);
        connect(buttons.getPassButton(), &QPushButton::clicked, this, &Game::passButton);
        connect(buttons.getClearButton(), &QPushButton::clicked, this, &Game::clearButton);
        connect(buttons.getShuffleButton(), &QPushButton::clicked, this, &Game::shuffleButton);
        connect(buttons.getSwapButton(), &QPushButton::clicked, this, &Game::swapButton);
        connect(buttons.getConfirmButton(), &QPushButton::clicked, this, &Game::confirmButton);
        connect(buttons.getCancelButton(), &QPushButton::clicked, this, &Game::cancelButton);
        connect(&selection, &Tile::letterAddedRemoved, this, &Game::setCorrectButtonState);

        // Board
        for (int collumn = 0; collumn < board.getGridDimensions().width(); ++collumn)
        {
            for (int row = 0; row < board.getGridDimensions().height(); ++row)
            {
                Tile* tile = board.getTileAtPosition(collumn, row);

                connect(tile, &Tile::proposeLetter, this, &Game::proposeLetter);
                connect(tile, &Tile::unproposeLetter, this, &Game::unproposeLetter);
                connect(tile, &Tile::wildcardPlacedOnBoard, this, &Game::assignWildcardLetter);
            }
        }

        // Hands
        for (auto& player : all_players)
        {
            for (int collumn = 0; collumn < settings->getHandDimensions().width(); ++collumn)
            {
                for (int row = 0; row < settings->getHandDimensions().height(); ++row)
                {
                    Tile* tile = player->getHand()->getTileAtPosition(collumn, row);

                    connect(tile, &Tile::markForSwap, this, &Game::addToSwapLetters);
                    connect(tile, &Tile::unmarkForSwap, this, &Game::removeFromSwapLetters);
                }
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
            buttons.showPlayClearSwapButtons();
        }
        else if (state == GameState::Play)
        {
            buttons.showPassShuffleSwapButtons();
        }
        else if (state == GameState::Swap)
        {
            buttons.showConfirmCancelButtons();
        }

        return;
    }
    
    bool Game::isPlacementValid()
    {
        if (!isPlacementLinear())
        {
            return false;
        }

        for (const auto letter : proposed_letters)
        {
            checked_tiles.clear();
            
            if (!isPlacementConnectedToStart(letter))
            {
                return false;
            }
        }

        return true;
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
            findProposedWordInLine(collumn, settings->getBoardDimensions().height(), Direction::Vertical);
        }

        for (const auto row : rows)
        {
            findProposedWordInLine(row, settings->getBoardDimensions().width(), Direction::Horisontal);
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

        for (auto& word : proposed_words)
        {
            if (!settings->getLanguage()->isInWordList(word.getWordAsText().toLower()))
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
    
    void Game::setGameState(GameState a_state)
    {
        state = a_state;

        switch (state)
        {
            case GameState::Play:
            {
                board.setDimmedAndDisabled(false);
                all_players[current_player_index]->getHand()->setTileInteractMode(TileInteractMode::Move);
                break;
            }
            case GameState::Swap:
            {
                board.setDimmedAndDisabled(true);
                all_players[current_player_index]->getHand()->setTileInteractMode(TileInteractMode::Swap);
                break;
            }
        }

        return;
    }
    
    void Game::assignWildcardLetter(Tile* a_tile)
    {
        if (a_tile->getLetter()->getWildcardText() != "")
        {
            return;
        }
        
        bool dialog_ok;
        
        QString wildcard_letter = QInputDialog::getItem
        (
            this,
            "Wordfeud",
            "Choose letter:",
            letter_pool.getNonWildcardLetters(),
            0,
            false,
            &dialog_ok
        );

        if (dialog_ok && !wildcard_letter.isEmpty())
        {
            a_tile->getLetter()->setWildcardText(wildcard_letter);
        }
        else
        {
            Letter* letter = a_tile->removeLetter();
            selection.placeLetter(letter);
        }

        return;
    }
    
    void Game::addToSwapLetters(Tile* a_tile)
    {
        swap_letters.push_back(a_tile);
        return;
    }
    
    void Game::removeFromSwapLetters(Tile* a_tile)
    {
        std::vector<Tile*>::iterator position = std::find(swap_letters.begin(), swap_letters.end(), a_tile);

        if (position != swap_letters.end())
        {
            swap_letters.erase(position);
        }

        return;
    }
    
    int Game::swapLetters()
    {
        std::vector<Letter*> swapped_letters;
        int swap_count = swap_letters.size();

        for (auto tile : swap_letters)
        {
            swapped_letters.push_back(tile->removeLetter());
        }

        all_players[current_player_index]->fillHand(&letter_pool);

        for (auto letter : swapped_letters)
        {
            letter_pool.insertLetter(letter);
        }

        return swap_count;
    }
    
    void Game::clearSwapList()
    {
        for (auto tile : swap_letters)
        {
            tile->setSwapMarking(false);
        }

        swap_letters.clear();
        all_players[current_player_index]->getHand()->repaint();

        return;
    }
    
    bool Game::isGameOver()
    {
        if (consecutive_passes >= 3)
        {
            return true;
        }

        for (auto& player : all_players)
        {
            if (player->getHand()->getLetterCount() == 0)
            {
                return true;
            }
        }

        return false;
    }
    
    Player* Game::getHighestScoringPlayer()
    {
        Player* highest_scoring_player = nullptr;
        int high_score = INT_MIN;
        
        for (auto& player : all_players)
        {
            if (player->getScore() > high_score)
            {
                high_score = player->getScore();
                highest_scoring_player = player;
            }
            else if (player->getScore() == high_score)
            {
                highest_scoring_player = nullptr;
            }
        }

        return highest_scoring_player;
    }
    
    void Game::finalisePoints()
    {
        for (auto& player : all_players)
        {
            player->addPoints(- player->getLetterPenaltyPoints());

            if (player->getHand()->getLetterCount() == 0)
            {
                for (auto& other_player : all_players)
                {
                    if (other_player == player)
                    {
                        continue;
                    }

                    player->addPoints(other_player->getLetterPenaltyPoints());
                }
            }
        }

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
}