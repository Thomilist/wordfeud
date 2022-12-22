#include "Settings.hpp"


namespace wf
{
    Settings::Settings()
    {
        
    }
    
    Settings::~Settings()
    { }
    
    void Settings::setGridDimensions(int rows, int collumns)
    {
        grid_dimensions.setHeight(rows);
        grid_dimensions.setWidth(collumns);

        return;
    }
    
    void Settings::setTileSize(int size)
    {
        tile_size.setHeight(size);
        tile_size.setWidth(size);

        return;
    }
    
    LanguageName Settings::getLanguage() const
    {
        return language;
    }
    
    const QSize& Settings::getGridDimensions() const
    {
        return grid_dimensions;
    }
    
    const QSize& Settings::getTileSize() const
    {
        return tile_size;
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
}