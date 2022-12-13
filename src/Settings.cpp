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
    
    LanguageName Settings::getLanguage()
    {
        return language;
    }
    
    const QSize& Settings::getGridDimensions()
    {
        return grid_dimensions;
    }
    
    const QSize& Settings::getTileSize()
    {
        return tile_size;
    }
}