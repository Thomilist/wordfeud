#include "Tile.hpp"

namespace wf
{
    Tile::Tile(const QSize& a_size)
    {
        QPixmap tile_background{":/tile-background.png"};
        setPixmap(tile_background.scaled(a_size.width(), a_size.height()));
    }
    
    Tile::~Tile()
    { }
}