#ifndef __BOARD_H__
#define __BOARD_H__

#include <QWidget>
#include <QGridLayout>
#include <QSize>
#include <QLayoutItem>

#include "ForwardDeclarations.hpp"
#include "Tile.hpp"
#include "BoardType.hpp"

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

            Tile* getTileAtPosition(int a_collumn, int a_row);
            QSize getGridDimensions();
            BoardType getType();

        private:
            void createEmptyGrid();
            void updateBoardSize();

            QGridLayout grid;
            QSize grid_dimensions;
            QSize tile_size;
            QSize board_size;
            Tile* selection;
            BoardType type;
    };
}
#endif // __BOARD_H__