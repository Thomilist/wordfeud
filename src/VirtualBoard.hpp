#ifndef __VIRTUALBOARD_H__
#define __VIRTUALBOARD_H__

#include <vector>

#include "ForwardDeclarations.hpp"

#include "VirtualTile.hpp"
#include "Board.hpp"

namespace wf
{
    class VirtualBoard
    {
        public:
            VirtualBoard();
            ~VirtualBoard();

            void setWithBoard(Board* a_board);
            VirtualTile* getVirtualTileAtPosition(int a_collumn, int a_row);
        
        private:
            std::vector<std::vector<VirtualTile>> board;
    };
}

#endif // __VIRTUALBOARD_H__