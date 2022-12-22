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
            LanguageName getLanguage() const;
            const QSize& getGridDimensions() const;
            const QSize& getTileSize() const;
            const QSize& getHandDimensions() const;
            const QSize& getHandTileSize() const;
            const QSize& getSelectionTileSize() const;

        private:
            LanguageName language{LanguageName::English};
            QSize grid_dimensions{15,15};
            QSize tile_size{42,42};
            QSize hand_dimensions{7,1};
            QSize hand_tile_size{90,90};
            QSize selection_tile_size{64,64};
    };
}
#endif // __SETTINGS_H__