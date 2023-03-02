#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include <algorithm>
#include <random>
#include <array>
#include <unordered_set>

#include <QWidget>
#include <QGridLayout>
#include <QSize>
#include <QMouseEvent>
#include <QPoint>
#include <QStackedWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QColor>
#include <QInputDialog>
#include <QChar>
#include <QString>

#include "ForwardDeclarations.hpp"

#include "Settings.hpp"
#include "Letter.hpp"
#include "Modifier.hpp"
#include "Board.hpp"
#include "Language.hpp"
#include "LetterPool.hpp"
#include "Player.hpp"
#include "ButtonPanel.hpp"
#include "Tile.hpp"
#include "Header.hpp"
#include "Word.hpp"
#include "ProposalInfo.hpp"
#include "ModifierPattern.hpp"

#include "BoardType.hpp"
#include "LanguageName.hpp"
#include "LetterStatus.hpp"
#include "ModifierType.hpp"
#include "PlayType.hpp"
#include "Direction.hpp"
#include "GameState.hpp"

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

        public slots:
            void playButton();
            void passButton();
            void clearButton();
            void shuffleButton();
            void swapButton();
            void confirmButton();
            void cancelButton();
            void proposeLetter(Tile* a_tile);
            void unproposeLetter(Tile* a_tile);
            void clearProposed();
            void setCorrectButtonState();
            void assignWildcardLetter(Tile* a_tile);
            void addToSwapLetters(Tile* a_tile);
            void removeFromSwapLetters(Tile* a_tile);

        private:
            void loadLetters();
            void placeLetter(int a_collumn, int a_row, Letter* a_letter);
            [[nodiscard]] Letter* removeLetter(int a_collumn, int a_row);
            void lockProposedLetters();
            void lockRecentlyLockedLetters();
            void placeModifier(int a_collumn, int a_row, Modifier* a_modifier, bool a_overwrite = false);
            void placeModifiers(std::vector<Modifier*> a_modifiers);
            void createPlayer(QString a_display_name, QColor a_color);
            void createPlayers();
            void deletePlayers();
            void nextPlayer();
            void initialiseConnections();
            void initialisePlayerConnections();
            std::vector<Letter*> getAllLetters();
            void mouseMoveEvent(QMouseEvent* a_event);
            void showCorrectButtons();
            bool isPlacementValid();
            bool isPlacementConnectedToStart(Tile* a_tile);
            bool isPlacementLinear();
            bool isPlacementInOneCollumn();
            bool isPlacementInOneRow();
            void findProposedWords();
            void findProposedWordInLine(int a_line, int a_max_index, Direction a_direction);
            void checkProposedWords();
            void findInvalidProposedWords();
            void calculateProposedPoints();
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
            std::vector<Tile*> proposed_letters;
            std::vector<Letter*> locked_letters;
            std::vector<Tile*> swap_letters;
            std::vector<Tile*> checked_tiles;
            std::vector<Word> proposed_words;
            std::vector<Word*> invalid_words;
            long unsigned int current_player_index = 0;
            Settings* settings;
            QGridLayout game_layout;
            Tile selection;
            Header header;
            Board board;
            ProposalInfo proposal_info;
            QStackedWidget hands;
            ButtonPanel buttons;
            std::default_random_engine rng;
            int proposed_words_points = 0;
            bool proposed_words_valid = true;
            int consecutive_passes = 0;
    };
}

#endif // __GAME_H__