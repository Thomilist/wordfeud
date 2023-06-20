#include "RenderedBoard.hpp"

namespace wf
{
    RenderedBoard::RenderedBoard(
        BoardType a_board_type,
        Settings* a_settings, 
        RenderedTile* a_selection,
        QWidget* a_parent)
        : QWidget(a_parent)
        , VirtualBoard(a_settings)
        , settings(a_settings)
        , selection(a_selection)
        , type(a_board_type)
    {
        setMouseTracking(true);
        grid.setSpacing(0);
        updateBoardSize();
        setMaximumSize(board_size.width(), board_size.height() * 1.1);
        setLayout(&grid);
        createEmptyGrid();
        assignNeighbours();
        adjustSize();
    }
    
    RenderedBoard::~RenderedBoard()
    { }
    
    void RenderedBoard::createEmptyGrid()
    {
        for (int row = 0; row < grid_dimensions.height(); ++row)
        {
            for (int column = 0; column < grid_dimensions.width(); ++column)
            {
                RenderedTile* tile = new RenderedTile(settings, selection, type, this);
                tile->setGridPosition(column, row);
                grid.addWidget(tile, row, column);
            }
        }
    }
    
    void RenderedBoard::updateBoardSize()
    {
        switch (type)
        {
            case BoardType::Board:
            {
                grid_dimensions = settings->getBoardDimensions();
                tile_size = settings->getBoardTileSize();
                break;
            }
            case BoardType::Hand:
            {
                grid_dimensions = settings->getHandDimensions();
                tile_size = settings->getHandTileSize();
                break;
            }
            case BoardType::Selection:
            {
                grid_dimensions.setHeight(0);
                grid_dimensions.setWidth(0);
                tile_size = settings->getSelectionTileSize();
                break;
            }
        }
        
        board_size.setHeight(grid_dimensions.height() * tile_size.height());
        board_size.setWidth(grid_dimensions.width() * tile_size.width());

        return;
    }
    
    RenderedTile* RenderedBoard::getTileAtPosition(int a_column, int a_row)
    {
        if (    a_column < 0
            ||  a_row < 0
            ||  a_column >= grid_dimensions.width()
            ||  a_row >= grid_dimensions.height())
        {
            return nullptr;
        }
        
        QLayoutItem* layout_item = grid.itemAtPosition(a_row, a_column);

        if (layout_item == nullptr)
        {
            return nullptr;
        }

        QWidget* widget = layout_item->widget();

        if (widget == nullptr)
        {
            return nullptr;
        }

        RenderedTile* tile = dynamic_cast<RenderedTile*>(widget);
        return tile;
    }
    
    RenderedTile* RenderedBoard::getTileAtPosition(QPoint a_position)
    {
        return getTileAtPosition(a_position.x(), a_position.y());
    }
    
    BoardType RenderedBoard::getType() const
    {
        return type;
    }
    
    void RenderedBoard::setDimmedAndDisabled(bool a_state)
    {
        dimmed_and_disabled = a_state;
        setDisabled(dimmed_and_disabled);

        for (int column = 0; column < getGridDimensions().width(); ++column)
        {
            for (int row = 0; row < getGridDimensions().height(); ++row)
            {
                getTileAtPosition(column, row)->setDimmed(dimmed_and_disabled);
            }
        }

        repaint();
        return;
    }
    
    void RenderedBoard::setTileInteractMode(TileInteractMode a_mode)
    {
        for (int column = 0; column < getGridDimensions().width(); ++column)
        {
            for (int row = 0; row < getGridDimensions().height(); ++row)
            {
                getTileAtPosition(column, row)->setInteractMode(a_mode);
            }
        }

        return;
    }
    
    void RenderedBoard::reset()
    {
        clearProposed();
        
        for (int column = 0; column < getGridDimensions().width(); ++column)
        {
            for (int row = 0; row < getGridDimensions().height(); ++row)
            {
                getTileAtPosition(column, row)->reset();
            }
        }

        setDimmedAndDisabled(false);
    }
}