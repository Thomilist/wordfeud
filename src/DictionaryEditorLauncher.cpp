#include "DictionaryEditorLauncher.hpp"


namespace wf
{
    DictionaryEditorLauncher::DictionaryEditorLauncher(
        AbstractEditor* a_editor,
        const QString& a_editor_subject,
        Settings* a_settings,
        QWidget* a_parent)
        : AbstractEditorLauncher(
            a_editor,
            a_editor_subject,
            a_settings,
            a_parent)
    { }
    
    DictionaryEditorLauncher::~DictionaryEditorLauncher()
    { }
    
    void DictionaryEditorLauncher::repopulateDropdown()
    {
        dropdown.clear();
        using_internal_languages = false;
        
        for (const auto& language : settings->getAvailableLanguagesAsStrings())
        {
            dropdown.addItem(language);
        }

        if (dropdown.count() == 0)
        {
            using_internal_languages = true;
            
            for (const auto& language : settings->getLoadedLanguages())
            {
                dropdown.addItem(language.getName());
            }
        }

        return;
    }
    
    bool DictionaryEditorLauncher::exportSelected()
    {
        if (export_destination_dialog.exec())
        {
            QString dictionary_name = dropdown.currentText();
            
            QDir dictionary_export_path = export_destination_dialog.directory();

            if (!dictionary_export_path.exists(dictionary_name))
            {
                dictionary_export_path.mkdir(dictionary_name);
            }

            QDir dictionary_export_subpath{dictionary_export_path.filePath(dictionary_name)};
            
            // Source files
            QString word_list_source;
            QString biased_words_source;
            QString letter_list_source;

            if (using_internal_languages)
            {
                Language* internal_language = settings->getLanguage(dictionary_name);
                word_list_source = internal_language->getWordListPath();
                biased_words_source = internal_language->getBiasedWordListPath();
                letter_list_source = internal_language->getLetterListPath();
            }
            else
            {
                word_list_source = Language::getWordListPath(dictionary_name);
                biased_words_source = Language::getBiasedWordListPath(dictionary_name);
                letter_list_source = Language::getLetterListPath(dictionary_name);
            }

            // Destination files
            QString word_list_destination = dictionary_export_subpath.filePath(dictionary_name % "-words.txt");
            QString biased_words_destination = dictionary_export_subpath.filePath(dictionary_name % "-biased-words.txt");
            QString letter_list_destination = dictionary_export_subpath.filePath(dictionary_name % "-letters.csv");

            // Copy to destination
            for (auto [source, destination] :
            {
                std::pair{word_list_source, word_list_destination},
                std::pair{biased_words_source, biased_words_destination},
                std::pair{letter_list_source, letter_list_destination}
            })
            {
                QFile source_file{source};

                if (source_file.exists())
                {
                    source_file.copy(destination);
                }
            }

            return true;
        }

        return false;
    }
    
    void DictionaryEditorLauncher::deleteSelected()
    {
        QDir dictionary_path{Language::getPath(dropdown.currentText())};
        dictionary_path.removeRecursively();
        return;
    }
    
    void DictionaryEditorLauncher::updateButtonState()
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
            button->setDisabled(using_internal_languages);

            if (using_internal_languages && button->isChecked())
            {
                allow_launch = false;
            }
        }

        buttons.button(QDialogButtonBox::Ok)->setEnabled(allow_launch);

        return;
    }
}