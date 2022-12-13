#ifndef __GAME_H__
#define __GAME_H__

#include <vector>

#include <QWidget>
#include <QGridLayout>

#include "ForwardDeclarations.hpp"
#include "Settings.hpp"
#include "Letter.hpp"
#include "Modifier.hpp"
#include "Board.hpp"
#include "Language.hpp"
#include "LetterPool.hpp"

namespace wf
{
    class Game : public QWidget
    {
        public:
            Game(Settings& a_settings, QWidget* a_parent = nullptr);
            ~Game();

            void loadLanguage();
            void setLetters();
            std::vector<Letter*> getLetters();

            LetterPool letter_pool;

        private:
            Settings settings;
            QGridLayout game_layout;
            QLabel header;
            Board board;
            QLabel hand;
            QLabel buttons;
            std::vector<Letter> all_letters;
            std::vector<Modifier> all_modifiers;
            Language language;
    };
}

#endif // __GAME_H__