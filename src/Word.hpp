#ifndef __WORD_H__
#define __WORD_H__

#include <vector>

#include <QString>

#include "ForwardDeclarations.hpp"
#include "Tile.hpp"
#include "Letter.hpp"
#include "Modifier.hpp"

#include "ModifierType.hpp"

namespace wf
{
    class Word
    {
        public:
            Word();
            ~Word();

            void appendLetter(Tile* a_tile);
            QString getWordAsText() const;
            int calculatePoints() const;
            std::vector<Tile*> getTiles() const;
            bool contains(Tile* a_tile) const;
            bool containsAnyOf(std::vector<Tile*> a_tiles) const;
            int containsHowManyOf(std::vector<Tile*> a_tiles) const;
            int getLength() const;
            void clear();

        private:
            std::vector<Tile*> tiles;
    };
}

#endif // __WORD_H__