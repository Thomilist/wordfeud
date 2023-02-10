#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <QSize>
#include <QFont>
#include <QFontDatabase>

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
            const QSize& getBoardDimensions() const;
            const QSize& getBoardTileSize() const;
            const QSize& getHandDimensions() const;
            const QSize& getHandTileSize() const;
            const QSize& getSelectionTileSize() const;
            QFont getMonospaceFont() const;

        private:
            LanguageName language{LanguageName::English};
            QSize board_dimensions{15,15};
            QSize board_tile_size{42,42};
            QSize hand_dimensions{7,1};
            QSize hand_tile_size{90,90};
            QSize selection_tile_size{64,64};
            QFont monospace_font;
    };
}
#endif // __SETTINGS_H__