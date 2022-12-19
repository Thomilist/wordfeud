#include "Board.hpp"

namespace wf
{
    Board::Board(
        BoardType a_board_type,
        const QSize& a_grid_dimensions,
        const QSize& a_tile_size, 
        Tile* a_selection,
        QWidget* a_parent)
        : QWidget(a_parent)
        , grid_dimensions(a_grid_dimensions)
        , tile_size(a_tile_size)
        , selection(a_selection)
        , type(a_board_type)
    {
        setMouseTracking(true);
        grid.setSpacing(0);
        updateBoardSize();
        setMaximumSize(board_size.width(), board_size.height() * 1.1);
        setLayout(&grid);
        createEmptyGrid();
        adjustSize();
    }
    
    Board::~Board()
    { }
    
    void Board::createEmptyGrid()
    {
        for (int row = 0; row < grid_dimensions.height(); ++row)
        {
            for (int collumn = 0; collumn < grid_dimensions.width(); ++collumn)
            {
                Tile* tile = new Tile(tile_size, selection, type, this);
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
    
    Tile* Board::getTileAtPosition(int a_collumn, int a_row)
    {
        QLayoutItem* layout_item = grid.itemAtPosition(a_row, a_collumn);

        if (layout_item == nullptr)
        {
            return nullptr;
        }

        QWidget* widget = layout_item->widget();

        if (widget == nullptr)
        {
            return nullptr;
        }

        Tile* tile = dynamic_cast<Tile*>(widget);
        return tile;
    }
    
    QSize Board::getGridDimensions()
    {
        return grid_dimensions;
    }
    
    BoardType Board::getType()
    {
        return type;
    }
}