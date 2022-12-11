#ifndef __TILE_H__
#define __TILE_H__

#include <QLabel>
#include <QSize>

namespace wf
{
    class Tile : public QLabel
    {
        Q_OBJECT

        public:
            Tile(const QSize& a_size);
            ~Tile();

        private:
    };
}

#endif // __TILE_H__