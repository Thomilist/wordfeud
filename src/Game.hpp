#ifndef __GAME_H__
#define __GAME_H__

#include <vector>

#include <QWidget>
#include <QGridLayout>
#include <QSize>
#include <QMouseEvent>
#include <QPoint>
#include <QStackedWidget>

#include "ForwardDeclarations.hpp"
#include "Settings.hpp"
#include "Letter.hpp"
#include "Modifier.hpp"
#include "Board.hpp"
#include "Language.hpp"
#include "LetterPool.hpp"
#include "Player.hpp"

#include "BoardType.hpp"
#include "LanguageName.hpp"
#include "LetterStatus.hpp"
#include "ModifierType.hpp"

namespace wf
{
    class Game : public QWidget
    {
        public:
            Game(Settings& a_settings, QWidget* a_parent = nullptr);
            ~Game();

            void loadLanguage();
            void loadLetters();
            void placeLetter(int a_collumn, int a_row, Letter* a_letter);
            [[nodiscard]] Letter* removeLetter(int a_collumn, int a_row);
            void lockLetters();
            void loadModifiers();
            void placeModifier(int a_collumn, int a_row, Modifier* a_modifier, bool a_overwrite = false);
            void placeModifiers(std::vector<Modifier*> a_modifiers);
            void createPlayer(QString a_display_name);
            void nextPlayer();

            LetterPool letter_pool;

        private:
            std::vector<Letter*> getAllLetters();
            std::vector<Modifier*> getAllModifiers();
            void mouseMoveEvent(QMouseEvent* a_event);

            std::vector<Letter> all_letters;
            std::vector<Modifier> all_modifiers;
            std::vector<Player*> all_players;
            long unsigned int current_player_index = 0;
            Settings settings;
            QGridLayout game_layout;
            QLabel header;
            Board board;
            QStackedWidget hands;
            QLabel buttons;
            Language language;
            Tile selection;
    };
}

#endif // __GAME_H__