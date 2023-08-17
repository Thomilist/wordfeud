#include "AbstractEditorLauncher.hpp"


namespace wf
{
    AbstractEditorLauncher::~AbstractEditorLauncher()
    { }
    
    void AbstractEditorLauncher::open()
    {
        repopulateDropdown();
        updateInterfaceState();
        QDialog::open();
        return;
    }

    AbstractEditorLauncher::AbstractEditorLauncher(
        AbstractEditor* a_editor,
        const QString& a_editor_subject,
        Settings* a_settings,
        QWidget* a_parent)
        : QDialog(a_parent, QDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint)
        , settings(a_settings)
        , export_destination_dialog(
            this,
            "Choose export destination",
            QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
        )
        , buttons(QDialogButtonBox::Help | QDialogButtonBox::Ok | QDialogButtonBox::Cancel)
    {
        setModal(true);
        setLayout(&grid_layout);
        layout()->setSizeConstraint(QLayout::SetFixedSize);

        setWindowTitle
        (
            QString()
            % "Open "
            % a_editor_subject.toLower()
            % " editor"
        );

        connect(&buttons, &QDialogButtonBox::accepted, this, &AbstractEditorLauncher::launch);
        connect(this, &AbstractEditorLauncher::launchEditor, a_editor, &AbstractEditor::launch);
        connect(&buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        connect(&buttons, &QDialogButtonBox::helpRequested, a_editor, &AbstractEditor::viewEditorHelp);
        connect(&radio_buttons, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &AbstractEditorLauncher::repopulateDropdown);
        connect(&radio_buttons, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &AbstractEditorLauncher::updateInterfaceState);

        initialiseRadioButtons();

        export_destination_dialog.setFileMode(QFileDialog::Directory);
        export_destination_dialog.setOption(QFileDialog::ShowDirsOnly, true);

        int layout_row = 0;

        grid_layout.addWidget(&create_new_button, layout_row++, 0);
        grid_layout.addWidget(&open_copy_button, layout_row++, 0);
        grid_layout.addWidget(&edit_existing_button, layout_row++, 0);
        grid_layout.addWidget(&export_existing_button, layout_row++, 0);
        grid_layout.addWidget(&delete_existing_button, layout_row++, 0);

        grid_layout.addWidget(&dropdown, layout_row++, 0);

        grid_layout.addWidget(&buttons, layout_row++, 0, Qt::AlignCenter);
    }
    
    void AbstractEditorLauncher::updateInterfaceState()
    {
        dropdown.setDisabled(create_new_button.isChecked());
        updateButtonText();
        updateButtonState();
        return;
    }
    
    void AbstractEditorLauncher::launch()
    {
        QString language_name = dropdown.currentText();
        EditorMode editor_mode = EditorMode::Undefined;
        
        for (auto [button, mode] :
        {
            std::pair{&create_new_button, EditorMode::CreateNew},
            std::pair{&open_copy_button, EditorMode::OpenCopy},
            std::pair{&edit_existing_button, EditorMode::EditExisting},
            std::pair{&export_existing_button, EditorMode::ExportExisting},
            std::pair{&delete_existing_button, EditorMode::DeleteExisting}
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
            case EditorMode::CreateNew:
            case EditorMode::OpenCopy:
            case EditorMode::EditExisting:
            {
                QDialog::accept();
                emit launchEditor(editor_mode, language_name);
                break;
            }
            case EditorMode::ExportExisting:
            {
                if (exportSelected())
                {
                    QDialog::accept();
                }

                break;
            }
            case EditorMode::DeleteExisting:
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
                    deleteSelected();
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
    
    void AbstractEditorLauncher::initialiseRadioButtons()
    {
        radio_buttons.addButton(&create_new_button);
        radio_buttons.addButton(&open_copy_button);
        radio_buttons.addButton(&edit_existing_button);
        radio_buttons.addButton(&export_existing_button);
        radio_buttons.addButton(&delete_existing_button);

        create_new_button.setChecked(true);

        return;
    }
    
    void AbstractEditorLauncher::updateButtonText()
    {
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

        return;
    }
}