#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <vector>

#include <QSize>
#include <QFont>
#include <QFontDatabase>

#include "ForwardDeclarations.hpp"
#include "Language.hpp"
#include "ModifierPattern.hpp"

namespace wf
{
    class Settings
    {
        public:
            Settings();
            ~Settings();

            void setGridDimensions(int a_rows, int a_collumns);
            void setTileSize(int a_size);
            void setLanguage(LanguageName a_language);
            void setLanguage(QString a_language);
            Language* getLanguage();
            std::vector<Language>& getAvailableLanguages();
            const QSize& getBoardDimensions() const;
            const QSize& getBoardTileSize() const;
            const QSize& getHandDimensions() const;
            const QSize& getHandTileSize() const;
            const QSize& getSelectionTileSize() const;
            QFont getMonospaceFont() const;
            ModifierPattern* getModifierPattern();
            

        private:
            std::vector<Language> languages;
            Language* current_language;
            QSize board_dimensions{15,15};
            QSize board_tile_size{42,42};
            QSize hand_dimensions{7,1};
            QSize hand_tile_size{90,90};
            QSize selection_tile_size{64,64};
            QFont monospace_font;
            ModifierPattern modifier_pattern;
    };
}
#endif // __SETTINGS_H__