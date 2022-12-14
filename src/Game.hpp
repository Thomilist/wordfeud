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

#include "BoardType.hpp"
#include "LanguageName.hpp"
#include "LetterStatus.hpp"
#include "ModifierType.hpp"
#include "PlayType.hpp"
#include "Direction.hpp"

namespace wf
{
    class Game : public QWidget
    {
        Q_OBJECT

        public:
            Game(
                Settings& a_settings,
                QWidget* a_parent = nullptr);
            ~Game();

            void loadLanguage();
            void loadLetters();
            void placeLetter(int a_collumn, int a_row, Letter* a_letter);
            [[nodiscard]] Letter* removeLetter(int a_collumn, int a_row);
            void lockProposedLetters();
            void lockRecentlyLockedLetters();
            void loadModifiers();
            void placeModifier(int a_collumn, int a_row, Modifier* a_modifier, bool a_overwrite = false);
            void placeModifiers(std::vector<Modifier*> a_modifiers);
            void createPlayer(QString a_display_name, QColor a_color);
            void nextPlayer();
        
        public slots:
            void play();
            void pass();
            void clear();
            void shuffle();
            void swap();
            void proposeLetter(Tile* a_tile);
            void unproposeLetter(Tile* a_tile);
            void clearProposed();
            void setCorrectButtonState();

        private:
            void initialiseConnections();
            std::vector<Letter*> getAllLetters();
            std::vector<Modifier*> getAllModifiers();
            void mouseMoveEvent(QMouseEvent* a_event);
            void showCorrectButtons();
            bool isPlacementValid();
            bool isPlacementConnectedToStart(Tile* a_tile);
            bool isPlacementLinear();
            bool isPlacementInOneCollumn();
            bool isPlacementInOneRow();
            void findProposedWords();
            void findProposedWordInLine(int a_line, int a_max_index, Direction a_direction);

            LetterPool letter_pool;
            std::vector<Letter> all_letters;
            std::vector<Modifier> all_modifiers;
            std::vector<Player*> all_players;
            std::vector<Tile*> proposed_letters;
            std::vector<Letter*> locked_letters;
            std::vector<Tile*> checked_tiles;
            std::vector<Word> proposed_words;
            long unsigned int current_player_index = 0;
            Settings settings;
            QGridLayout game_layout;
            Header header;
            Board board;
            QStackedWidget hands;
            ButtonPanel buttons;
            Language language;
            Tile selection;
            std::default_random_engine rng;
    };
}

#endif // __GAME_H__