#include "DictionaryEditorLauncher.hpp"


namespace wf
{
    DictionaryEditorLauncher::DictionaryEditorLauncher(Settings* a_settings, QWidget* a_parent)
        : QDialog(a_parent, QDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint)
        , settings(a_settings)
        , buttons(QDialogButtonBox::Help | QDialogButtonBox::Ok | QDialogButtonBox::Cancel)
        , choose_dictionary_label("Choose dictionary:")
        , create_new_button("Create new")
        , open_copy_button("Open copy of existing")
        , edit_existing_button("Edit existing")
        , delete_existing_button("Delete existing")
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

        int layout_row = 0;

        grid_layout.addWidget(&choose_dictionary_label, layout_row++, 0);

        grid_layout.addWidget(&create_new_button, layout_row++, 0);
        grid_layout.addWidget(&open_copy_button, layout_row++, 0);
        grid_layout.addWidget(&edit_existing_button, layout_row++, 0);
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
        
        for (auto [button, mode] :
        {
            std::pair{&create_new_button, DictionaryEditorMode::CreateNew},
            std::pair{&open_copy_button, DictionaryEditorMode::OpenCopy},
            std::pair{&edit_existing_button, DictionaryEditorMode::EditExisting},
        })
        {
            if (button->isChecked())
            {
                QDialog::accept();
                emit launchDictionaryEditor(mode, language_name);
                return;
            }
        }

        if (delete_existing_button.isChecked())
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
                return;
            }
        }

        return;
    }
    
    void DictionaryEditorLauncher::repopulateDictionaryDropdown(QAbstractButton*)
    {
        dictionary_dropdown.clear();
        
        for (const auto& language : settings->getAvailableLanguagesAsStrings())
        {
            dictionary_dropdown.addItem(language);
        }

        return;
    }
    
    void DictionaryEditorLauncher::updateInteractState()
    {
        dictionary_dropdown.setEnabled(!create_new_button.isChecked());

        for (auto [button, text] : 
        {
            std::pair{&create_new_button, "Create"},
            std::pair{&open_copy_button, "Open Copy"},
            std::pair{&edit_existing_button, "Open"},
            std::pair{&delete_existing_button, "Delete"},
        })
        {
            if (button->isChecked())
            {
                buttons.button(QDialogButtonBox::Ok)->setText(text);
            }
        }

        return;
    }
    
    void DictionaryEditorLauncher::initialiseRadioButtons()
    {
        radio_buttons.addButton(&create_new_button);
        radio_buttons.addButton(&open_copy_button);
        radio_buttons.addButton(&edit_existing_button);
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
    
    void DictionaryEditorLauncher::deleteDictionary(QString a_language)
    {
        QDir dictionary_path{Language::getPath(a_language)};
        dictionary_path.removeRecursively();
        return;
    }
}