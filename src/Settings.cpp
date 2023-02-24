#include "Settings.hpp"


namespace wf
{
    Settings::Settings()
        : languages{
            Language{LanguageName::Danish},
            Language{LanguageName::English}}
    {
        setLanguage(LanguageName::English);
        
        int monospace_font_family_id = QFontDatabase::addApplicationFont(":/fonts/NotoSansMono-Regular.ttf");
        QString monospace_font_family = QFontDatabase::applicationFontFamilies(monospace_font_family_id).at(0);
        monospace_font.setFamily(monospace_font_family);
        monospace_font.setStyleHint(QFont::TypeWriter);
    }
    
    Settings::~Settings()
    { }
    
    void Settings::setGridDimensions(int a_rows, int a_collumns)
    {
        board_dimensions.setHeight(a_rows);
        board_dimensions.setWidth(a_collumns);

        return;
    }
    
    void Settings::setTileSize(int size)
    {
        board_tile_size.setHeight(size);
        board_tile_size.setWidth(size);

        return;
    }
    
    void Settings::setLanguage(LanguageName a_language)
    {
        for (auto& language : languages)
        {
            if (language.asEnum() == a_language)
            {
                current_language = &language;
                return;
            }
        }

        return;
    }
    
    void Settings::setLanguage(QString a_language)
    {
        for (auto& language : languages)
        {
            if (language.asString() == a_language)
            {
                current_language = &language;
                return;
            }
        }

        return;
    }
    
    Language* Settings::getLanguage()
    {
        return current_language;
    }
    
    std::vector<Language>& Settings::getAvailableLanguages()
    {
        return languages;
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
    
    ModifierPattern* Settings::getModifierPattern()
    {
        return &modifier_pattern;
    }
    
    void Settings::setLeftPlayer(QString a_player_name, PlayerType a_player_type)
    {
        left_player_name = a_player_name;
        left_player_type = a_player_type;
        return;
    }
    
    void Settings::setRightPlayer(QString a_player_name, PlayerType a_player_type)
    {
        right_player_name = a_player_name;
        right_player_type = a_player_type;
        return;
    }
    
    const std::pair<QString, PlayerType> Settings::getLeftPlayer() const
    {
        return {left_player_name, left_player_type};
    }
    
    const std::pair<QString, PlayerType> Settings::getRightPlayer() const
    {
        return {right_player_name, right_player_type};
    }
}