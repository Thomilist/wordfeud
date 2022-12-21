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

#include "BoardType.hpp"
#include "LetterStatus.hpp"
#include "ModifierType.hpp"

namespace wf
{
    class Tile : public QWidget
    {
        Q_OBJECT
        
        public:
            Tile(
                const QSize& a_size,
                Tile* a_selection,
                BoardType a_board_type,
                QWidget* a_parent = nullptr,
                bool a_follows_mouse = false);
            ~Tile();

            void placeLetter(Letter* a_letter);
            [[nodiscard]] Letter* removeLetter();
            Letter* getLetter();
            void setModifier(Modifier* a_modifier);
            Modifier* getModifier();
        
        signals:
            void propose_letter(Tile* a_tile);
            void unpropose_letter(Tile* a_tile);

        private:
            void paintEvent(QPaintEvent* a_event);
            void mousePressEvent(QMouseEvent* a_event);
            
            Modifier* modifier = nullptr;
            Letter* letter = nullptr;
            Tile* selection;
            bool follows_mouse;
            BoardType type;

            // Dimensions, percentage of tile size
            int margin = 5; 
            int radius = 20;
    };
}

#endif // __TILE_H__