#ifndef __TILE_H__
#define __TILE_H__

#include <iostream>

#include <QWidget>
#include <QSize>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QPoint>
#include <QColor>
#include <QFont>
#include <QMouseEvent>

#include "ForwardDeclarations.hpp"
#include "Letter.hpp"
#include "Modifier.hpp"

namespace wf
{
    class Tile : public QWidget
    {
        Q_OBJECT

        public:
            Tile(const QSize& a_size, Tile* a_selection, QWidget* a_parent = nullptr, bool a_follows_mouse = false);
            ~Tile();

            void placeLetter(Letter* a_letter);
            void lockLetter();
            [[nodiscard]] Letter* removeLetter();
            Letter* getLetter();
            void setModifier(Modifier* a_modifier);
            Modifier* getModifier();

        private:
            void paintEvent(QPaintEvent* a_event);
            void mousePressEvent(QMouseEvent* a_event);
            
            Modifier* modifier = nullptr;
            Letter* letter = nullptr;
            Tile* selection;
            bool follows_mouse;

            // Dimensions, percentage of tile size
            int margin = 5; 
            int radius = 20;
    };
}

#endif // __TILE_H__