#ifndef __WORD_H__
#define __WORD_H__

#include <vector>

#include <QString>

#include "ForwardDeclarations.hpp"

#include "Letter.hpp"
#include "Modifier.hpp"
#include "VirtualTile.hpp"

#include "ModifierType.hpp"

namespace wf
{
    class Word
    {
        public:
            Word();
            ~Word();

            void appendLetter(VirtualTile* a_tile);
            QString getWordAsText() const;
            int calculatePoints() const;
            std::vector<VirtualTile*> getTiles() const;
            bool contains(VirtualTile* a_tile) const;
            bool containsAnyOf(std::vector<VirtualTile*> a_tiles) const;
            int containsHowManyOf(std::vector<VirtualTile*> a_tiles) const;
            int getLength() const;
            void clear();

        private:
            std::vector<VirtualTile*> tiles;
    };
}

#endif // __WORD_H__