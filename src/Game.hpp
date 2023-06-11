#ifndef __GAME_H__
#define __GAME_H__

#include <algorithm>
#include <array>
#include <random>
#include <unordered_set>
#include <vector>

#include <QChar>
#include <QColor>
#include <QGridLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPoint>
#include <QPushButton>
#include <QSize>
#include <QStackedWidget>
#include <QString>
#include <QThread>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "ButtonPanel.hpp"
#include "Header.hpp"
#include "Language.hpp"
#include "Letter.hpp"
#include "LetterPool.hpp"
#include "Modifier.hpp"
#include "ModifierPattern.hpp"
#include "Player.hpp"
#include "PlayerAI.hpp"
#include "ProposalInfo.hpp"
#include "RenderedBoard.hpp"
#include "RenderedTile.hpp"
#include "Settings.hpp"
#include "Word.hpp"

#include "BoardType.hpp"
#include "Direction.hpp"
#include "GameState.hpp"
#include "LanguageName.hpp"
#include "LetterStatus.hpp"
#include "ModifierType.hpp"
#include "PlayerType.hpp"
#include "PlayType.hpp"

namespace wf
{
    class Game : public QWidget
    {
        Q_OBJECT

        public:
            Game(
                Settings* a_settings,
                QWidget* a_parent = nullptr);
            ~Game();
        
            void reset();
            void terminatePlayerAI();

        public slots:
            void playButton();
            void passButton();
            void clearButton();
            void shuffleButton();
            void swapButton();
            void confirmButton();
            void cancelButton();
            void proposeLetter(RenderedTile* a_tile);
            void unproposeLetter(RenderedTile* a_tile);
            void clearProposed();
            void setCorrectButtonState();
            void assignWildcardLetter(RenderedTile* a_tile);
            void addToSwapLetters(RenderedTile* a_tile);
            void removeFromSwapLetters(RenderedTile* a_tile);
            void repaintHandAndBoard();
        
        signals:
            void playAI();

        private:
            void loadLetters();
            void placeLetter(int a_collumn, int a_row, Letter* a_letter);
            [[nodiscard]] Letter* removeLetter(int a_collumn, int a_row);
            void placeModifier(int a_collumn, int a_row, Modifier* a_modifier, bool a_overwrite = false);
            void placeModifiers(std::vector<Modifier*> a_modifiers);
            void createPlayer(QString a_display_name, PlayerType a_type, QColor a_color);
            void createPlayers();
            void deletePlayers();
            void nextPlayer();
            void initialiseConnections();
            void initialisePlayerConnections();
            std::vector<Letter*> getAllLetters();
            void mouseMoveEvent(QMouseEvent* a_event);
            void showCorrectButtons();
            void displayProposedPlayValue();
            void setGameState(GameState a_state);
            int swapLetters();
            void clearSwapList();
            bool isGameOver();
            Player* getHighestScoringPlayer();
            void finalisePoints();

            GameState state = GameState::Play;
            LetterPool letter_pool;
            std::vector<Letter> all_letters;
            std::vector<Player*> all_players;
            std::vector<RenderedTile*> swap_letters;
            long unsigned int current_player_index = 0;
            Settings* settings;
            QGridLayout game_layout;
            RenderedTile selection;
            Header header;
            RenderedBoard board;
            ProposalInfo proposal_info;
            QStackedWidget hands;
            ButtonPanel buttons;
            std::default_random_engine rng;
            int consecutive_passes = 0;
            QThread player_AI_thread;
    };
}

#endif // __GAME_H__