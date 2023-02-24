#include "VirtualBoard.hpp"


namespace wf
{
    VirtualBoard::VirtualBoard()
    { }
    
    VirtualBoard::~VirtualBoard()
    { }
    
    void VirtualBoard::setWithBoard(Board* a_board)
    {
        board.clear();
        std::vector<VirtualTile> tile_collumn;
        
        for (int collumn = 0; collumn < a_board->getGridDimensions().width(); ++collumn)
        {
            for (int row = 0; row < a_board->getGridDimensions().height(); ++row)
            {
                tile_collumn.push_back(VirtualTile(a_board->getTileAtPosition(collumn, row)));
            }

            board.push_back(tile_collumn);
            tile_collumn.clear();
        }

        return;
    }
    
    VirtualTile* VirtualBoard::getVirtualTileAtPosition(int a_collumn, int a_row)
    {
        return &board[a_collumn][a_row];
    }
}