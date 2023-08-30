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
    { }
    
    BoardEditorLauncher::~BoardEditorLauncher()
    { }
    
    bool BoardEditorLauncher::exportSelected()
    {
        if (export_destination_dialog.exec())
        {
            QString modifier_pattern_name = dropdown.currentText();
            
            QDir modifier_pattern_export_path = export_destination_dialog.directory();

            if (!modifier_pattern_export_path.exists(modifier_pattern_name))
            {
                modifier_pattern_export_path.mkdir(modifier_pattern_name);
            }

            // Source file
            QString modifier_pattern_source;

            if (using_internal_modifier_patterns)
            {
                ModifierPattern* internal_modifier_pattern = settings->getModifierPattern(modifier_pattern_name);
                modifier_pattern_source = internal_modifier_pattern->getPath();
            }
            else
            {
                modifier_pattern_source = ModifierPattern::getPath(modifier_pattern_name);
            }

            // Destination file
            QString modifier_pattern_destination = modifier_pattern_export_path.filePath(modifier_pattern_name % ".json");

            // Copy
            QFile source_file{modifier_pattern_source};

            if (source_file.exists())
            {
                source_file.copy(modifier_pattern_destination);
            }

            return true;
        }

        return false;
    }
    
    void BoardEditorLauncher::deleteSelected()
    {
        QFile modifier_pattern_path{ModifierPattern::getPath(dropdown.currentText())};
        modifier_pattern_path.remove();
        return;
    }
    
    void BoardEditorLauncher::updateButtonState()
    {
        bool allow_launch = true;

        // Prevent launch if the dictionary dropdown is empty and something other than "Create new" is selected
        allow_launch = !(dropdown.currentText().isEmpty() && !create_new_button.isChecked());

        for (auto button :
        {
            &open_copy_button,
            &edit_existing_button,
            &delete_existing_button
        })
        {
            button->setDisabled(using_internal_modifier_patterns);

            if (using_internal_modifier_patterns && button->isChecked())
            {
                allow_launch = false;
            }
        }

        buttons.button(QDialogButtonBox::Ok)->setEnabled(allow_launch);

        return;
    }
    
    void BoardEditorLauncher::repopulateDropdown()
    {
        dropdown.clear();
        using_internal_modifier_patterns = false;
        
        for (const auto& pattern : settings->getAvailableModifierPatternsAsStrings())
        {
            dropdown.addItem(pattern);
        }

        if (dropdown.count() == 0)
        {
            using_internal_modifier_patterns = true;
            
            for (const auto& pattern : settings->getLoadedModifierPatterns())
            {
                dropdown.addItem(pattern.getName());
            }
        }

        return;
    }
}