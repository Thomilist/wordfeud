#include "VirtualTile.hpp"


namespace wf
{
    VirtualTile::VirtualTile(Tile* a_tile)
        : modifier(a_tile->getModifier())
        , letter(a_tile->getLetter())
    { }
    
    VirtualTile::~VirtualTile()
    { }
    
    Modifier* VirtualTile::getModifier()
    {
        return modifier;
    }
    
    Letter* VirtualTile::getLetter()
    {
        return letter;
    }
}