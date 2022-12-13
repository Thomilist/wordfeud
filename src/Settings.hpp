#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <QSize>

#include "ForwardDeclarations.hpp"
#include "Language.hpp"

namespace wf
{
    class Settings
    {
        public:
            Settings();
            ~Settings();

            void setGridDimensions(int rows, int collumns);
            void setTileSize(int size);
            LanguageName getLanguage();
            const QSize& getGridDimensions();
            const QSize& getTileSize();

        private:
            LanguageName language{LanguageName::English};
            QSize grid_dimensions{15,15};
            QSize tile_size{32,32};
    };
}
#endif // __SETTINGS_H__