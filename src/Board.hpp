#ifndef __BOARD_H__
#define __BOARD_H__

#include <QWidget>
#include <QGridLayout>
#include <QSize>
#include <QLayoutItem>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <QPoint>
#include <QColor>

#include "ForwardDeclarations.hpp"
#include "Tile.hpp"
#include "BoardType.hpp"
#include "TileNeighbours.hpp"
#include "TileInteractMode.hpp"

namespace wf
{
    class Board : public QWidget
    {
        public:
            Board(
                BoardType a_board_type,
                const QSize& a_grid_dimensions,
                const QSize& a_tile_size,
                Tile* a_selection,
                QWidget* a_parent = nullptr);
            ~Board();

            Tile* getTileAtPosition(int a_collumn, int a_row) const;
            QSize getGridDimensions() const;
            BoardType getType() const;
            void setDimmedAndDisabled(bool a_state);
            void setTileInteractMode(TileInteractMode a_mode);
            int getTileCount();
            int getLetterCount();

        private:
            void createEmptyGrid();
            void updateBoardSize();
            void assignNeighbours();

            QGridLayout grid;
            QSize grid_dimensions;
            QSize tile_size;
            QSize board_size;
            Tile* selection;
            BoardType type;
            bool dimmed_and_disabled = false;

    };
}
#endif // __BOARD_H__