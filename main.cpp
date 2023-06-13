/*
wordfeud clone
version 0.1.0
https://github.com/Thomilist/wordfeud

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2022-2023 Thomas Emil Jensen

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
#include <QCoreApplication>
#include <QFile>
#include <QMainWindow>
#include <QObject>
#include <QTextStream>

#include "src/ForwardDeclarations.hpp"

#include "src/Wordfeud.hpp"

int main(int argc, char* argv[])
{
    QApplication application{argc, argv};

    QCoreApplication::setOrganizationName("Thomilist");
    QCoreApplication::setApplicationName("wordfeud");
    QCoreApplication::setApplicationVersion("0.2.0");

    QFile stylesheet{":/dark/stylesheet.qss"};
    stylesheet.open(QFile::ReadOnly | QFile::Text);
    QTextStream stylesheet_stream(&stylesheet);
    application.setStyleSheet(stylesheet_stream.readAll());

    wf::Wordfeud wordfeud;
    QObject::connect(&application, &QApplication::aboutToQuit, &wordfeud, &wf::Wordfeud::closeWindow);

    return application.exec();
}