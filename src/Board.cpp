#include "Board.hpp"

namespace wf
{
    Board::Board(
        BoardType a_board_type,
        Settings* a_settings, 
        Tile* a_selection,
        QWidget* a_parent)
        : QWidget(a_parent)
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
    
    Board::~Board()
    {
        for (int row = 0; row < grid_dimensions.height(); ++row)
        {
            for (int collumn = 0; collumn < grid_dimensions.width(); ++collumn)
            {
                delete getTileAtPosition(collumn, row);
            }
        }
    }
    
    void Board::createEmptyGrid()
    {
        for (int row = 0; row < grid_dimensions.height(); ++row)
        {
            for (int collumn = 0; collumn < grid_dimensions.width(); ++collumn)
            {
                Tile* tile = new Tile(settings, selection, type, this);
                tile->setGridPosition(collumn, row);
                grid.addWidget(tile, row, collumn);
            }
        }
    }
    
    void Board::updateBoardSize()
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
    
    void Board::assignNeighbours()
    {
        for (int collumn = 0; collumn < grid_dimensions.width(); ++collumn)
        {
            for (int row = 0; row < grid_dimensions.height(); ++row)
            {
                Tile* tile = getTileAtPosition(collumn, row);

                tile->setNeighbour(getTileAtPosition(collumn, row - 1), TileNeighbour::Top);
                tile->setNeighbour(getTileAtPosition(collumn + 1, row), TileNeighbour::Right);
                tile->setNeighbour(getTileAtPosition(collumn, row + 1), TileNeighbour::Bottom);
                tile->setNeighbour(getTileAtPosition(collumn - 1, row), TileNeighbour::Left);
            }
        }

        return;
    }
    
    Tile* Board::getTileAtPosition(int a_collumn, int a_row) const
    {
        if (    a_collumn < 0
            ||  a_row < 0
            ||  a_collumn >= grid_dimensions.width()
            ||  a_row >= grid_dimensions.height())
        {
            return nullptr;
        }
        
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
    
    QSize Board::getGridDimensions() const
    {
        return grid_dimensions;
    }
    
    BoardType Board::getType() const
    {
        return type;
    }
    
    void Board::setDimmedAndDisabled(bool a_state)
    {
        dimmed_and_disabled = a_state;
        setDisabled(dimmed_and_disabled);

        for (int collumn = 0; collumn < getGridDimensions().width(); ++collumn)
        {
            for (int row = 0; row < getGridDimensions().height(); ++row)
            {
                getTileAtPosition(collumn, row)->setDimmed(dimmed_and_disabled);
            }
        }

        repaint();
        return;
    }
    
    void Board::setTileInteractMode(TileInteractMode a_mode)
    {
        for (int collumn = 0; collumn < getGridDimensions().width(); ++collumn)
        {
            for (int row = 0; row < getGridDimensions().height(); ++row)
            {
                getTileAtPosition(collumn, row)->setInteractMode(a_mode);
            }
        }

        return;
    }
    
    int Board::getTileCount()
    {
        return getGridDimensions().width() * getGridDimensions().height();
    }
    
    int Board::getLetterCount()
    {
        int count = 0;
        Letter* letter;
        
        for (int collumn = 0; collumn < getGridDimensions().width(); ++collumn)
        {
            for (int row = 0; row < getGridDimensions().height(); ++row)
            {
                letter = getTileAtPosition(collumn, row)->getLetter();

                if (letter != nullptr)
                {
                    ++count;
                }
            }
        }

        return count;
    }
}