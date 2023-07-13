#include "VirtualTile.hpp"


namespace wf
{
    VirtualTile::VirtualTile()
    { }

    VirtualTile::VirtualTile(VirtualTile* a_tile)
        : modifier(a_tile->getModifier())
        , letter(a_tile->getLetter())
    { }
    
    VirtualTile::~VirtualTile()
    { }
    
    void VirtualTile::placeLetter(Letter* a_letter)
    {
        if (letter == nullptr)
        {
            letter = a_letter;
        }
        
        return;
    }
    
    Letter* VirtualTile::removeLetter()
    {
        if (    letter->getStatus() == LetterStatus::Locked 
            ||  letter->getStatus() == LetterStatus::LockedRecently)
        {
            return nullptr;
        }
        
        Letter* current_letter = letter;
        letter = nullptr;

        // LetterStatus old_status = current_letter->getStatus();
        current_letter->setStatus(LetterStatus::Free);

        return current_letter;
    }
    
    void VirtualTile::overwriteLetter(Letter* a_letter)
    {
        letter = a_letter;
        return;
    }
    
    void VirtualTile::setModifier(Modifier* a_modifier)
    {
        modifier = a_modifier;
        return;
    }
    
    Modifier* VirtualTile::getModifier() const
    {
        return modifier;
    }
    
    Letter* VirtualTile::getLetter() const
    {
        return letter;
    }
    
    std::array<VirtualTile*, 4> VirtualTile::getNeighbours() const
    {
        return neighbours;
    }
    
    VirtualTile* VirtualTile::getNeighbour(TileNeighbour a_index) const
    {
        return neighbours[a_index];
    }
    
    void VirtualTile::setNeighbour(VirtualTile* a_tile, TileNeighbour a_index)
    {
        neighbours[a_index] = a_tile;
        return;
    }
    
    void VirtualTile::setGridPosition(int a_collumn, int a_row)
    {
        grid_position = QPoint{a_collumn, a_row};
        return;
    }
    
    QPoint VirtualTile::getGridPosition() const
    {
        return grid_position;
    }
}