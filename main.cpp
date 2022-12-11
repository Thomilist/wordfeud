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

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include "src/Board.hpp"
#include "src/Settings.hpp"

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    QMainWindow main_window;
    QWidget base_widget{&main_window};
    QGridLayout game_layout{&base_widget};

    wf::Settings settings;

    QLabel header{"Header", &base_widget};
    wf::Board board(settings.getGridDimensions(), settings.getTileSize(), &base_widget);
    QLabel hand{"Hand", &base_widget};
    QLabel buttons{"Buttons", &base_widget};

    game_layout.addWidget(&header, 0, 0);
    game_layout.addWidget(&board, 1, 0);
    game_layout.addWidget(&hand, 2, 0);
    game_layout.addWidget(&buttons, 3, 0);

    base_widget.setLayout(&game_layout);

    main_window.setCentralWidget(&base_widget);
    main_window.show();

    return application.exec();
}