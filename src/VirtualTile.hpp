#ifndef __VIRTUALTILE_H__
#define __VIRTUALTILE_H__

#include "ForwardDeclarations.hpp"

#include "Tile.hpp"
#include "Letter.hpp"
#include "Modifier.hpp"

namespace wf
{
    class VirtualTile
    {
        public:
            VirtualTile(Tile* a_tile);
            ~VirtualTile();

            Modifier* getModifier();
            Letter* getLetter();
        
        private:
            Modifier* modifier;
            Letter* letter;
    };
}

#endif // __VIRTUALTILE_H__