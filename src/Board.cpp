#include "Board.hpp"

namespace wf
{
    Board::Board(
        const QSize& a_grid_dimensions,
        const QSize& a_tile_size,
        QWidget* a_parent)
        : QWidget(a_parent)
        , grid_dimensions(a_grid_dimensions)
        , tile_size(a_tile_size)
    {
        grid.setSpacing(0);
        updateBoardSize();
        setMaximumSize(board_size);
        setLayout(&grid);
        createEmptyGrid();
    }
    
    Board::~Board()
    { }
    
    void Board::createEmptyGrid()
    {
        for (int row = 0; row < grid_dimensions.height(); ++row)
        {
            for (int collumn = 0; collumn < grid_dimensions.width(); ++collumn)
            {
                Tile* tile = new Tile(tile_size);
                grid.addWidget(tile, row, collumn);
            }
        }
    }
    
    void Board::updateBoardSize()
    {
        board_size.setHeight(grid_dimensions.height() * tile_size.height());
        board_size.setWidth(grid_dimensions.width() * tile_size.width());

        return;
    }
}