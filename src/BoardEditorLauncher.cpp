#include "BoardEditorLauncher.hpp"


namespace wf
{
    BoardEditorLauncher::BoardEditorLauncher(
        AbstractEditor* a_editor,
        Settings* a_settings,
        QWidget* a_parent)
        : AbstractEditorLauncher(
            a_editor,
            "Board",
            a_settings,
            a_parent)
    {
        
    }
    
    BoardEditorLauncher::~BoardEditorLauncher()
    {
        
    }
    
    bool BoardEditorLauncher::exportSelected()
    {
        return true;
    }
    
    void BoardEditorLauncher::deleteSelected()
    {
        
    }
    
    void BoardEditorLauncher::updateButtonState()
    {
        
    }
    
    void BoardEditorLauncher::repopulateDropdown()
    {
        
    }
}