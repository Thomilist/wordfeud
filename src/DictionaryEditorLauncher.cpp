#include "DictionaryEditorLauncher.hpp"


namespace wf
{
    DictionaryEditorLauncher::DictionaryEditorLauncher(Settings* a_settings, QWidget* a_parent)
        : QDialog(a_parent, QDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint)
        , settings(a_settings)
        , buttons(QDialogButtonBox::Help | QDialogButtonBox::Ok | QDialogButtonBox::Cancel)
        , export_destination_dialog(
            this,
            "Choose export destination",
            QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
        )
    {
        setModal(true);
        setLayout(&grid_layout);
        layout()->setSizeConstraint(QLayout::SetFixedSize);
        setWindowTitle("Open dictionary editor");

        connect(&buttons, &QDialogButtonBox::accepted, this, &DictionaryEditorLauncher::launch);
        connect(&buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        connect(&buttons, &QDialogButtonBox::helpRequested, this, &DictionaryEditorLauncher::viewEditorHelp);
        connect(&radio_buttons, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &DictionaryEditorLauncher::repopulateDictionaryDropdown);
        connect(&radio_buttons, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &DictionaryEditorLauncher::updateInteractState);

        initialiseRadioButtons();
        initaliseDictionaryDropdown();

        export_destination_dialog.setFileMode(QFileDialog::Directory);
        export_destination_dialog.setOption(QFileDialog::ShowDirsOnly, true);

        int layout_row = 0;

        grid_layout.addWidget(&choose_dictionary_label, layout_row++, 0);

        grid_layout.addWidget(&create_new_button, layout_row++, 0);
        grid_layout.addWidget(&open_copy_button, layout_row++, 0);
        grid_layout.addWidget(&edit_existing_button, layout_row++, 0);
        grid_layout.addWidget(&export_existing_button, layout_row++, 0);
        grid_layout.addWidget(&delete_existing_button, layout_row++, 0);

        grid_layout.addWidget(&dictionary_dropdown, layout_row++, 0);

        grid_layout.addWidget(&buttons, layout_row++, 0, Qt::AlignCenter);
    }
    
    DictionaryEditorLauncher::~DictionaryEditorLauncher()
    { }
    
    void DictionaryEditorLauncher::open()
    {
        repopulateDictionaryDropdown(nullptr);
        QDialog::open();
        return;
    }
    
    void DictionaryEditorLauncher::launch()
    {
        QString language_name = dictionary_dropdown.currentText();
        DictionaryEditorMode editor_mode = DictionaryEditorMode::Undefined;
        
        for (auto [button, mode] :
        {
            std::pair{&create_new_button, DictionaryEditorMode::CreateNew},
            std::pair{&open_copy_button, DictionaryEditorMode::OpenCopy},
            std::pair{&edit_existing_button, DictionaryEditorMode::EditExisting},
            std::pair{&export_existing_button, DictionaryEditorMode::ExportExisting},
            std::pair{&delete_existing_button, DictionaryEditorMode::DeleteExisting}
        })
        {
            if (button->isChecked())
            {
                editor_mode = mode;
                break;
            }
        }

        switch (editor_mode)
        {
            case DictionaryEditorMode::CreateNew:
            case DictionaryEditorMode::OpenCopy:
            case DictionaryEditorMode::EditExisting:
            {
                QDialog::accept();
                emit launchDictionaryEditor(editor_mode, language_name);
                break;
            }
            case DictionaryEditorMode::ExportExisting:
            {
                if (exportDictionary(language_name))
                {
                    QDialog::accept();
                }

                break;
            }
            case DictionaryEditorMode::DeleteExisting:
            {
                QMessageBox::StandardButton response = QMessageBox::Yes;

                response = QMessageBox::warning
                (
                    this,
                    "Delete dictionary",
                    QString() % "Are you sure you want to delete \"" % language_name % "\"?",
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes
                );

                if (response == QMessageBox::Yes)
                {
                    QDialog::accept();
                    deleteDictionary(language_name);
                    break;
                }

                break;
            }
            default:
            {
                break;
            }
        }

        return;
    }
    
    void DictionaryEditorLauncher::repopulateDictionaryDropdown(QAbstractButton*)
    {
        dictionary_dropdown.clear();
        using_internal_languages = false;
        
        for (const auto& language : settings->getAvailableLanguagesAsStrings())
        {
            dictionary_dropdown.addItem(language);
        }

        if (dictionary_dropdown.count() == 0)
        {
            using_internal_languages = true;
            
            for (const auto& language : settings->getLoadedLanguages())
            {
                dictionary_dropdown.addItem(language.getName());
            }
        }

        return;
    }
    
    void DictionaryEditorLauncher::updateInteractState()
    {
        bool allow_launch = true;

        // Prevent launch if the dictionary dropdown is empty and something other than "Create new" is selected
        allow_launch = !(dictionary_dropdown.currentText().isEmpty() && !create_new_button.isChecked());
        
        dictionary_dropdown.setDisabled(create_new_button.isChecked());

        for (auto [button, text] : 
        {
            std::pair{&create_new_button, "Create"},
            std::pair{&open_copy_button, "Open Copy"},
            std::pair{&edit_existing_button, "Open"},
            std::pair{&export_existing_button, "Export"},
            std::pair{&delete_existing_button, "Delete"}
        })
        {
            if (button->isChecked())
            {
                buttons.button(QDialogButtonBox::Ok)->setText(text);
            }
        }

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
    
    void DictionaryEditorLauncher::initialiseRadioButtons()
    {
        radio_buttons.addButton(&create_new_button);
        radio_buttons.addButton(&open_copy_button);
        radio_buttons.addButton(&edit_existing_button);
        radio_buttons.addButton(&export_existing_button);
        radio_buttons.addButton(&delete_existing_button);

        create_new_button.setChecked(true);

        return;
    }
    
    void DictionaryEditorLauncher::initaliseDictionaryDropdown()
    {
        repopulateDictionaryDropdown(nullptr);
        updateInteractState();
        return;
    }
    
    bool DictionaryEditorLauncher::exportDictionary(QString a_language)
    {
        if (export_destination_dialog.exec())
        {
            QDir dictionary_export_path = export_destination_dialog.directory();

            if (!dictionary_export_path.exists(a_language))
            {
                dictionary_export_path.mkdir(a_language);
            }

            QDir dictionary_export_subpath{dictionary_export_path.filePath(a_language)};
            
            // Source files
            QString word_list_source;
            QString biased_words_source;
            QString letter_list_source;

            if (using_internal_languages)
            {
                Language* internal_language = settings->getLanguage(a_language);
                word_list_source = internal_language->getWordListPath();
                biased_words_source = internal_language->getBiasedWordListPath();
                letter_list_source = internal_language->getLetterListPath();
            }
            else
            {
                word_list_source = Language::getWordListPath(a_language);
                biased_words_source = Language::getBiasedWordListPath(a_language);
                letter_list_source = Language::getLetterListPath(a_language);
            }

            // Destination files
            QString word_list_destination = dictionary_export_subpath.filePath(a_language % "-words.txt");
            QString biased_words_destination = dictionary_export_subpath.filePath(a_language % "-biased-words.txt");
            QString letter_list_destination = dictionary_export_subpath.filePath(a_language % "-letters.csv");

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
    
    void DictionaryEditorLauncher::deleteDictionary(QString a_language)
    {
        QDir dictionary_path{Language::getPath(a_language)};
        dictionary_path.removeRecursively();
        return;
    }
}