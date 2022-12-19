#ifndef __BOARD_H__
#define __BOARD_H__

#include <QWidget>
#include <QGridLayout>
#include <QSize>
#include <QLayoutItem>

#include "ForwardDeclarations.hpp"
#include "Tile.hpp"

namespace wf
{
    class Board : public QWidget
    {
        public:
            Board(const QSize& a_grid_dimensions, const QSize& a_tile_size, QWidget* a_parent = nullptr);
            ~Board();

            Tile* getTileAtPosition(int a_collumn, int a_row);
            QSize getGridDimensions();

        private:
            void createEmptyGrid();
            void updateBoardSize();

            QGridLayout grid;
            QSize grid_dimensions;
            QSize tile_size;
            QSize board_size;
    };
}
#endif // __BOARD_H__