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

        game_layout.addWidget(&header, grid_row++, 0, Qt::AlignCenter);
        game_layout.addWidget(&board, grid_row++, 0, Qt::AlignCenter);
        game_layout.addWidget(&proposal_info, grid_row++, 0, Qt::AlignCenter);
        game_layout.addWidget(&hands, grid_row++, 0, Qt::AlignCenter);
        game_layout.addWidget(&buttons, grid_row++, 0, Qt::AlignCenter);

        game_layout.setSpacing(0);
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
        player_AI_thread.start();

        if (all_players[current_player_index]->getType() == PlayerType::AI)
        {
            emit playAI();
        }
    }
    
    Game::~Game()
    {
        player_AI_thread.quit();
        player_AI_thread.wait();
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
    
    void Game::placeLetter(int a_column, int a_row, Letter* a_letter)
    {
        RenderedTile* tile = board.getTileAtPosition(a_column, a_row);

        if (tile == nullptr)
        {
            return;
        }

        tile->placeLetter(a_letter);

        return;
    }
    
    Letter* Game::removeLetter(int a_column, int a_row)
    {
        RenderedTile* tile = board.getTileAtPosition(a_column, a_row);

        if (tile == nullptr)
        {
            return nullptr;
        }

        return tile->removeLetter();
    }
    
    void Game::placeModifier(int a_column, int a_row, Modifier* a_modifier, bool a_overwrite)
    {
        RenderedTile* tile = board.getTileAtPosition(a_column, a_row);

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
            for (int column = 0; column < grid_dimensions.width(); ++column)
            {
                RenderedTile* tile = board.getTileAtPosition(column, row);

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
    
    void Game::createPlayer(QString a_display_name, PlayerType a_type, QColor a_color, int a_index)
    {
        Player* player = nullptr;
        
        switch (a_type)
        {
            case PlayerType::AI:
            {
                player = new PlayerAI
                (
                    a_display_name,
                    a_color,
                    settings,
                    &selection,
                    &board,
                    &letter_pool,
                    a_index
                );

                player->moveToThread(&player_AI_thread);

                break;
            }
            case PlayerType::Human:
            {
                player = new Player
                (
                    a_display_name,
                    a_type,
                    a_color,
                    settings,
                    &selection,
                    a_index
                );
                
                break;
            }
        }

        hands.addWidget(player->getHandCentered());
        all_players.push_back(player);
        return;
    }
    
    void Game::createPlayers()
    {
        PlayerSettings* left_player = settings->getLeftPlayer();
        PlayerSettings* right_player = settings->getRightPlayer();
        
        createPlayer(left_player->getName(), left_player->getType(), QColor{128, 0, 0}, 0);
        createPlayer(right_player->getName(), right_player->getType(), QColor{0, 0, 128}, 1);
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
        if (all_players.empty())
        {
            return;
        }

        all_players[current_player_index]->setTurn(false);
        displayProposedPlayValue();

        if (isGameOver())
        {
            hands.setDisabled(true);
            finalisePoints();
            repaint();
            saveScores();
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
            if (++current_player_index >= all_players.size())
            {
                current_player_index = 0;
            }

            all_players[current_player_index]->fillHand(&letter_pool);
            all_players[current_player_index]->setTurn(true);

            hands.setCurrentIndex(current_player_index);
        }
        
        settings->midGameApply();
        setCorrectButtonState();
        showCorrectButtons();
        repaint();

        if (all_players[current_player_index]->getType() == PlayerType::AI)
        {
            emit playAI();
        }

        return;
    }
    
    void Game::reset()
    {
        // Cleanup last game
        terminatePlayerAI();
        
        state = GameState::Play;
        current_player_index = 0;
        consecutive_passes = 0;

        all_letters.clear();
        swap_letters.clear();

        deletePlayers();

        header.reset();
        board.reset();
        selection.reset();
        proposal_info.setProposedPlay(true, 0);
        hands.setDisabled(false);

        // Initialise new game
        settings->newGameApply();
        createPlayers();
        initialisePlayerConnections();

        header.setLeftPlayer(all_players[0]);
        header.setRightPlayer(all_players[1]);
        all_players[current_player_index]->setTurn(true);

        loadLetters();
        letter_pool.set(getAllLetters());

        settings->getModifierPattern()->reset();
        placeModifiers(settings->getModifierPattern()->getModifiers());

        for (auto& player : all_players)
        {
            player->fillHand(&letter_pool);
        }

        buttons.setTileCount(letter_pool.getRemainingCount());
        setCorrectButtonState();
        showCorrectButtons();
        repaint();

        player_AI_thread.start();

        if (all_players[current_player_index]->getType() == PlayerType::AI)
        {
            emit playAI();
        }

        return;
    }
    
    void Game::terminatePlayerAI()
    {
        for (auto player : all_players)
        {
            if (player->getType() == PlayerType::AI)
            {
                auto player_ai = dynamic_cast<PlayerAI*>(player);
                player_ai->cancelTurn();
            }
        }

        player_AI_thread.quit();
        player_AI_thread.wait();
        return;
    }
    
    RecordTracker* Game::getRecords()
    {
        return &record_tracker;
    }
    
    void Game::playButton()
    {
        board.evaluateProposedPlay();
        
        // Placement check
        if (!board.isPlacementValid())
        {
            QMessageBox invalid_placement_warning;
            invalid_placement_warning.setText("Invalid placement.");
            invalid_placement_warning.setIcon(QMessageBox::Icon::Warning);
            invalid_placement_warning.setWindowTitle(" ");
            invalid_placement_warning.exec();
            return;
        }

        // Dictionary check
        if (!board.areProposedWordsValid())
        {
            auto invalid_words = board.getInvalidProposedWords();
            
            QString invalid_words_message{"Word(s) not found in dictionary:"};

            for (const auto& word : invalid_words)
            {
                invalid_words_message.append("\n");
                invalid_words_message.append(word.getWordAsText());
            }
            
            QMessageBox invalid_words_warning;
            invalid_words_warning.setText(invalid_words_message);
            invalid_words_warning.setIcon(QMessageBox::Icon::Warning);
            invalid_words_warning.setWindowTitle(" ");
            invalid_words_warning.exec();
            return;
        }

        // Points calculation
        int points = board.getProposedPlayPoints();
        all_players[current_player_index]->addPoints(points);

        // Find most relevant word
        Word most_relevant_word = board.getMostRelevantWord();
        
        // Cleanup
        board.lockProposedPlay();

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
        board.lockRecentlyLockedLetters();
        ++consecutive_passes;
        header.updateWithPlay(
            PlayType::Pass,
            all_players[current_player_index]);
        nextPlayer();
        return;
    }
    
    void Game::clearButton()
    {
        clearProposed();
        board.repaint();
        all_players[current_player_index]->getHand()->repaint();

        return;
    }
    
    void Game::shuffleButton()
    {
        RenderedBoard* hand = all_players[current_player_index]->getHand();
        QSize hand_size = hand->size();
        std::vector<RenderedTile*> tiles;
        std::vector<Letter*> letters;

        for (int row = 0; row < hand_size.width(); ++row)
        {
            for (int column = 0; column < hand_size.height(); ++column)
            {
                RenderedTile* tile = hand->getTileAtPosition(column, row);

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
    
    void Game::proposeLetter(RenderedTile* a_tile)
    {
        board.proposeLetter(a_tile);
        showCorrectButtons();
        setCorrectButtonState();
        displayProposedPlayValue();
        return;
    }
    
    void Game::unproposeLetter(RenderedTile* a_tile)
    {
        board.unproposeLetter(a_tile);
        showCorrectButtons();
        setCorrectButtonState();
        displayProposedPlayValue();
        return;
    }
    
    void Game::clearProposed()
    {
        auto letters = board.clearProposed();

        for (auto letter : letters)
        {
            all_players[current_player_index]->addLetterToHand(letter);
        }

        showCorrectButtons();
        setCorrectButtonState();
        displayProposedPlayValue();
        return;
    }
    
    void Game::setCorrectButtonState()
    {
        if (isGameOver())
        {
            buttons.getPlayButton()->setDisabled(true);
            buttons.getPassButton()->setDisabled(true);
            buttons.getClearButton()->setDisabled(true);
            buttons.getShuffleButton()->setDisabled(true);
            buttons.getSwapButton()->setDisabled(true);
            buttons.getConfirmButton()->setDisabled(true);
            buttons.getCancelButton()->setDisabled(true);
            return;
        }
        
        bool button_state = selection.getLetter() != nullptr;

        buttons.getPlayButton()->setDisabled(button_state);
        buttons.getPassButton()->setDisabled(button_state);
        buttons.getClearButton()->setDisabled(button_state);
        buttons.getShuffleButton()->setDisabled(button_state);
        buttons.getConfirmButton()->setDisabled(button_state);
        buttons.getCancelButton()->setDisabled(button_state);

        RenderedBoard* current_hand = all_players[current_player_index]->getHand();
        
        bool swap_button_state
            =   button_state 
            ||  (letter_pool.getRemainingCount() < (current_hand->getTileCount()))
            ||  (board.getProposedLetterCount() != 0);

        buttons.getSwapButton()->setDisabled(swap_button_state);

        bool confirm_button_state = true;
        
        for (int column = 0; column < current_hand->getGridDimensions().width(); ++column)
        {
            for (int row = 0; row < current_hand->getGridDimensions().height(); ++row)
            {
                if (current_hand->getTileAtPosition(column, row)->getSwapMarking() == true)
                {
                    confirm_button_state = false;
                    goto l_found_swap_marking;
                }
            }
        }

        l_found_swap_marking:
        {
            buttons.getConfirmButton()->setDisabled(confirm_button_state);
        }

        bool disable_player_input = all_players[current_player_index]->getType() == PlayerType::AI;
        hands.setDisabled(disable_player_input);
        buttons.setDisabled(disable_player_input);
        board.setDisabled(disable_player_input);

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
        connect(&selection, &RenderedTile::letterAddedRemoved, this, &Game::setCorrectButtonState);

        // Board
        for (int column = 0; column < board.getGridDimensions().width(); ++column)
        {
            for (int row = 0; row < board.getGridDimensions().height(); ++row)
            {
                RenderedTile* tile = board.getTileAtPosition(column, row);

                connect(tile, &RenderedTile::proposeLetter, this, &Game::proposeLetter);
                connect(tile, &RenderedTile::unproposeLetter, this, &Game::unproposeLetter);
                connect(tile, &RenderedTile::wildcardPlacedOnBoard, this, &Game::assignWildcardLetter);
            }
        }

        initialisePlayerConnections();

        return;
    }
    
    void Game::initialisePlayerConnections()
    {
        for (auto player : all_players)
        {
            // Hands
            for (int column = 0; column < settings->getHandDimensions().width(); ++column)
            {
                for (int row = 0; row < settings->getHandDimensions().height(); ++row)
                {
                    RenderedTile* tile = player->getHand()->getTileAtPosition(column, row);

                    connect(tile, &RenderedTile::markForSwap, this, &Game::addToSwapLetters);
                    connect(tile, &RenderedTile::unmarkForSwap, this, &Game::removeFromSwapLetters);
                }
            }

            // Player AI threading
            if (player->getType() == PlayerType::AI)
            {
                PlayerAI* player_ai = dynamic_cast<PlayerAI*>(player);

                connect(this, &Game::playAI, player_ai, &PlayerAI::playIfTurn);
                connect(this, &Game::cancelAI, player_ai, &PlayerAI::cancelTurn);
                connect(player_ai, &PlayerAI::playComplete, this, &Game::playButton);
                connect(player_ai, &PlayerAI::letterPlaced, this, &Game::repaintHandAndBoard);
                connect(player_ai, &PlayerAI::startSwap, this, &Game::swapButton);
                connect(player_ai, &PlayerAI::letterMarkedForSwap, this, &Game::repaintHandAndBoard);
                connect(player_ai, &PlayerAI::swapComplete, this, &Game::confirmButton);
                connect(player_ai, &PlayerAI::passTurn, this, &Game::passButton);
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
        if (board.getProposedLetterCount() != 0)
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
    
    void Game::displayProposedPlayValue()
    {
        board.evaluateProposedPlay();
        proposal_info.setProposedPlay(board.isProposedPlayValid(), board.getProposedPlayPoints());
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
    
    void Game::assignWildcardLetter(RenderedTile* a_tile)
    {
        if (a_tile->getLetter()->getWildcardText().isLetter())
        {
            return;
        }
        
        bool dialog_ok;
        
        QString wildcard_string = QInputDialog::getItem
        (
            this,
            "Wordfeud",
            "Choose letter:",
            letter_pool.getNonWildcardLetters(),
            0,
            false,
            &dialog_ok
        );

        QChar wildcard_letter = wildcard_string.at(0);

        if (dialog_ok && wildcard_letter.isLetter())
        {
            a_tile->getLetter()->setWildcardText(wildcard_letter);
        }
        else
        {
            Letter* letter = a_tile->removeLetter();
            selection.placeLetter(letter);
        }

        displayProposedPlayValue();

        return;
    }
    
    void Game::addToSwapLetters(RenderedTile* a_tile)
    {
        swap_letters.push_back(a_tile);
        setCorrectButtonState();
        return;
    }
    
    void Game::removeFromSwapLetters(RenderedTile* a_tile)
    {
        std::vector<RenderedTile*>::iterator position = std::find(swap_letters.begin(), swap_letters.end(), a_tile);

        if (position != swap_letters.end())
        {
            swap_letters.erase(position);
            setCorrectButtonState();
        }

        return;
    }
    
    void Game::repaintHandAndBoard()
    {
        all_players[current_player_index]->getHand()->repaint();
        board.repaint();
        selection.repaint();
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
            if (    player->getHand()->getLetterCount() == 0 
                &&  board.getProposedLetterCount() == 0
                &&  letter_pool.getRemainingCount() == 0)
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
    
    void Game::saveScores()
    {
        Player* winning_player = getHighestScoringPlayer();
        Score score;

        for (auto player : all_players)
        {
            score.name = player->getDisplayName();
            score.player_type = player->getType();
            score.points = player->getScore();
            score.timestamp = QDateTime::currentDateTime().toString(RecordTracker::getDateTimeFormat());
            score.dictionary = settings->getLanguage()->asString();
            score.modifier_pattern = settings->getModifierPattern()->getDistributionAsText();

            if (player == winning_player)
            {
                score.result = "Win";
            }
            else if (winning_player == nullptr)
            {
                score.result = "Draw";
            }
            else
            {
                score.result = "Loss";
            }

            if (player->getType() == PlayerType::AI)
            {
                PlayerAI* player_ai = dynamic_cast<PlayerAI*>(player);
                score.difficulty = player_ai->getDifficulty();
            }
            else
            {
                score.difficulty = 0;
            }

            record_tracker.insert(score);
        }
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