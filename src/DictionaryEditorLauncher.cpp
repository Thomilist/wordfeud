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
        setWindowTitle("Open dictionary editor");

        connect(&buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
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
        QDialog::open();
        return;
    }
    
    void DictionaryEditorLauncher::launch()
    {
        for (auto [button, mode] :
        {
            std::pair{&create_new_button, DictionaryEditorMode::CreateNew},
            std::pair{&open_copy_button, DictionaryEditorMode::OpenCopy},
            std::pair{&edit_existing_button, DictionaryEditorMode::EditExisting},
        })
        {
            if (button->isChecked())
            {
                emit launchDictionaryEditor(mode, dictionary_dropdown.currentText());
                break;
            }
        }

        if (delete_existing_button.isChecked())
        {
            // Are you sure?
        }

        return;
    }
    
    void DictionaryEditorLauncher::repopulateDictionaryDropdown(QAbstractButton*)
    {
        dictionary_dropdown.clear();
        
        for (const auto& language : settings->getAvailableLanguages())
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
            std::pair{&create_new_button, "Create New"},
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
}