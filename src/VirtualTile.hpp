#ifndef __VIRTUALTILE_H__
#define __VIRTUALTILE_H__

#include <array>

#include <QPoint>

#include "ForwardDeclarations.hpp"

#include "Letter.hpp"
#include "Modifier.hpp"

#include "TileNeighbours.hpp"

namespace wf
{
    class VirtualTile
    {
        public:
            VirtualTile();
            VirtualTile(VirtualTile* a_tile);
            ~VirtualTile();

            virtual void placeLetter(Letter* a_letter);
            [[nodiscard]] virtual Letter* removeLetter();
            void overwriteLetter(Letter* a_letter);
            void setModifier(Modifier* a_modifier);
            Modifier* getModifier() const;
            Letter* getLetter() const;
            std::array<VirtualTile*, 4> getNeighbours() const;
            VirtualTile* getNeighbour(TileNeighbour a_index) const;
            void setNeighbour(VirtualTile* a_tile, TileNeighbour a_index);
            void setGridPosition(int a_collumn, int a_row);
            const QPoint& getGridPosition() const;
        
        protected:
            Modifier* modifier = nullptr;
            Letter* letter = nullptr;
            QPoint grid_position;

            // Top, right, bottom, left
            std::array<VirtualTile*, 4> neighbours{nullptr, nullptr, nullptr, nullptr};

    };
}

#endif // __VIRTUALTILE_H__