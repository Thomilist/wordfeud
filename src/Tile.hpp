#ifndef __TILE_H__
#define __TILE_H__

#include <array>
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
#include "Settings.hpp"

#include "BoardType.hpp"
#include "LetterStatus.hpp"
#include "ModifierType.hpp"
#include "TileNeighbours.hpp"
#include "TileInteractMode.hpp"

namespace wf
{
    class Tile : public QWidget
    {
        Q_OBJECT
        
        public:
            Tile(
                Settings* a_settings,
                Tile* a_selection,
                BoardType a_board_type,
                QWidget* a_parent = nullptr,
                bool a_follows_mouse = false);
            ~Tile();

            void placeLetter(Letter* a_letter);
            [[nodiscard]] Letter* removeLetter();
            Letter* getLetter() const;
            void setModifier(Modifier* a_modifier);
            Modifier* getModifier() const;
            std::array<Tile*, 4> getNeighbours() const;
            Tile* getNeighbour(TileNeighbour a_index) const;
            void setNeighbour(Tile* a_tile, TileNeighbour a_index);
            void setGridPosition(int a_collumn, int a_row);
            QPoint getGridPosition() const;
            BoardType getBoardType() const;
            void setDimmed(bool a_state);
            void setInteractMode(TileInteractMode a_mode);
            void setSwapMarking(bool a_state);
            void reset();
        
        signals:
            void proposeLetter(Tile* a_tile);
            void unproposeLetter(Tile* a_tile);
            void letterAddedRemoved();
            void wildcardPlacedOnBoard(Tile* a_tile);
            void markForSwap(Tile* a_tile);
            void unmarkForSwap(Tile* a_tile);

        private:
            void paintEvent(QPaintEvent* a_event);
            void mousePressEvent(QMouseEvent* a_event);
            
            Settings* settings;
            QSize tile_size;
            Modifier* modifier = nullptr;
            Letter* letter = nullptr;
            Tile* selection;
            bool follows_mouse;
            BoardType board_type;
            QPoint grid_position;
            bool dimmed = false;
            TileInteractMode interact_mode = TileInteractMode::Move;
            bool swap_marking = false;

            // Top, right, bottom, left
            std::array<Tile*, 4> neighbours{nullptr, nullptr, nullptr, nullptr};

            // Dimensions, percentage of tile size
            int margin = 5; 
            int radius = 20;
    };
}

#endif // __TILE_H__