#ifndef __TILE_H__
#define __TILE_H__

#include <QLabel>
#include <QSize>

#include "Letter.hpp"
#include "Modifier.hpp"

namespace wf
{
    class Tile : public QLabel
    {
        Q_OBJECT

        public:
            Tile(const QSize& a_size);
            ~Tile();

            void placeLetter(Letter a_letter);
            void lockLetter();
            void removeLetter();
            void getLetter();

        private:
            Modifier modifier;
            Letter letter;
    };
}

#endif // __TILE_H__