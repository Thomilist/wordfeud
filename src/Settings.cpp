#include "Settings.hpp"


namespace wf
{
    Settings::Settings()
    {
        int monospace_font_family_id = QFontDatabase::addApplicationFont(":/fonts/NotoSansMono-Regular.ttf");
        QString monospace_font_family = QFontDatabase::applicationFontFamilies(monospace_font_family_id).at(0);
        monospace_font.setFamily(monospace_font_family);
        monospace_font.setStyleHint(QFont::TypeWriter);
    }
    
    Settings::~Settings()
    { }
    
    void Settings::setGridDimensions(int rows, int collumns)
    {
        board_dimensions.setHeight(rows);
        board_dimensions.setWidth(collumns);

        return;
    }
    
    void Settings::setTileSize(int size)
    {
        board_tile_size.setHeight(size);
        board_tile_size.setWidth(size);

        return;
    }
    
    LanguageName Settings::getLanguage() const
    {
        return language;
    }
    
    const QSize& Settings::getBoardDimensions() const
    {
        return board_dimensions;
    }
    
    const QSize& Settings::getBoardTileSize() const
    {
        return board_tile_size;
    }
    
    const QSize& Settings::getHandDimensions() const
    {
        return hand_dimensions;
    }
    
    const QSize& Settings::getHandTileSize() const
    {
        return hand_tile_size;
    }
    
    const QSize& Settings::getSelectionTileSize() const
    {
        return selection_tile_size;
    }
    
    QFont Settings::getMonospaceFont() const
    {
        return monospace_font;
    }
}