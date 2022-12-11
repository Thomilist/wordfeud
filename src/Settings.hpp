#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <QSize>

namespace wf
{
    class Settings
    {
        public:
            Settings();
            ~Settings();

            void setGridDimensions(int rows, int collumns);
            void setTileSize(int size);
            const QSize& getGridDimensions();
            const QSize& getTileSize();

        private:

            QSize grid_dimensions{15,15};
            QSize tile_size{32,32};
    };
}
#endif // __SETTINGS_H__