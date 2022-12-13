/*
wordfeud clone
version 1.0.0
https://github.com/Thomilist/wordfeud

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2022 Thomas Emil Jensen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include "src/Tile.hpp"
#include "src/Board.hpp"
#include "src/Settings.hpp"
#include "src/Language.hpp"
#include "src/Letter.hpp"
#include "src/LetterPool.hpp"
#include "src/Game.hpp"

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    QMainWindow main_window;
    wf::Settings settings;

    wf::Game game{settings, &main_window};

    main_window.setCentralWidget(&game);
    main_window.show();

    int i = 0;

    while (game.letter_pool.getRemainingCount())
    {
        wf::Letter* random_letter = game.letter_pool.getRandomLetter();
        std::cout << ++i << " " << random_letter->getText().toStdString() << " " << random_letter->getPoints() << "\n";
    }

    return application.exec();
}