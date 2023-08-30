#include "AbstractEditor.hpp"


namespace wf
{
    void AbstractEditor::launch(EditorMode a_mode, QString a_name)
    {
        mode = a_mode;
        original_item = a_name;
        name_edit.setText(original_item);

        prepare();

        QDialog::open();
        return;
    }
    
    void AbstractEditor::viewEditorHelp()
    {
        QDesktopServices::openUrl(help_url);
        return;
    }

    AbstractEditor::AbstractEditor(
        const QString& a_editor_subject,
        Settings* a_settings,
        QWidget* a_parent)
        : QDialog(a_parent, QDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint)
        , settings(a_settings)
        , button_box(QDialogButtonBox::Help | QDialogButtonBox::Save | QDialogButtonBox::Discard)
        , editor_subject(a_editor_subject)
    {
        setModal(true);
        setLayout(&grid_layout);
        layout()->setSizeConstraint(QLayout::SetFixedSize);

        setEditorTitle(editor_subject % " Editor");
        setRejectTitle("Discard " % editor_subject.toLower());
        setOverwriteTitle("Overwrite " % editor_subject.toLower());

        button_box.button(QDialogButtonBox::Save)->setText("Save && Close");

        connect(&button_box, &QDialogButtonBox::accepted, this, &AbstractEditor::saveAndClose);
        connect(button_box.button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &QDialog::reject);
        connect(&button_box, &QDialogButtonBox::helpRequested, this, &AbstractEditor::viewEditorHelp);

        header_font.setPixelSize(30);

        createNameGroup();

        int layout_row = 0;

        grid_layout.addWidget(&header, layout_row++, 0);
        grid_layout.addLayout(&body_layout, layout_row++, 0);
        grid_layout.addWidget(&name_group, layout_row++, 0);
        grid_layout.addWidget(&button_box, layout_row++, 0, Qt::AlignCenter);

        setMouseTracking(true);
        header.setMouseTracking(true);
        button_box.setMouseTracking(true);
        name_edit.setMouseTracking(true);
    }
    
    QGridLayout* AbstractEditor::getBodyLayout()
    {
        return &body_layout;
    }
    
    void AbstractEditor::setEditorTitle(const QString& a_title)
    {
        setWindowTitle(a_title);
        return;
    }

    void AbstractEditor::setRejectTitle(const QString& a_title)
    {
        reject_title = a_title;
        return;
    }
    
    void AbstractEditor::setRejectBody(const QString& a_body)
    {
        reject_body = a_body;
        return;
    }
    
    void AbstractEditor::setOverwriteTitle(const QString& a_title)
    {
        overwrite_title = a_title;
        return;
    }
    
    void AbstractEditor::setHelpURL(const QString& a_url)
    {
        help_url.setUrl(a_url);
        return;
    }
    
    void AbstractEditor::setExistingItems(const std::set<QString>& a_items)
    {
        existing_items = a_items;
        return;
    }
    
    void AbstractEditor::setCurrentItem(const QString& a_item)
    {
        current_item = a_item;
        return;
    }
    
    void AbstractEditor::updateHeaderText()
    {
        switch (mode)
        {
            case EditorMode::CreateNew:
            {
                header.setText
                (
                    QString()
                    % "Creating New "
                    % editor_subject
                );

                break;
            }
            case EditorMode::OpenCopy:
            {
                header.setText
                (
                    QString()
                    % "Editing Copy of \""
                    % original_item
                    % "\""
                );

                break;
            }
            case EditorMode::EditExisting:
            {
                header.setText
                (
                    QString()
                    % "Editing \""
                    % original_item
                    % "\""
                );

                break;
            }
            case EditorMode::ExportExisting:
            {
                header.setText
                (
                    QString()
                    % "Exporting \""
                    % original_item
                    % "\""
                    % " - how are you seeing this?"
                );

                break;
            }
            case EditorMode::DeleteExisting:
            {
                header.setText
                (
                    QString()
                    % "Deleting \""
                    % original_item
                    % "\""
                    % " - how are you seeing this?"
                );

                break;
            }
            default:
            {
                break;
            }
        }

        header.setFont(header_font);

        return;
    }
    
    void AbstractEditor::updateNameSuggestion()
    {
        switch (mode)
        {
            case EditorMode::CreateNew:
            {
                setCurrentItem(getUniqueName("New " % editor_subject));
                break;
            }
            case EditorMode::OpenCopy:
            {
                setCurrentItem(getUniqueName(original_item % "_Copy"));
                break;
            }
            default:
            {
                break;
            }
        }

        name_edit.setText(current_item);

        return;
    }

    void AbstractEditor::reject()
    {
        QMessageBox::StandardButton response = QMessageBox::Yes;

        response = QMessageBox::warning
        (
            this,
            reject_title,
            reject_body,
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes
        );

        if (response == QMessageBox::Yes)
        {
            QDialog::reject();
        }

        return;
    }
    
    void AbstractEditor::saveAndClose()
    {
        if (promptOverwrite() == QMessageBox::Yes)
        {
            save();
            QDialog::accept();
        }
        
        return;
    }
    
    void AbstractEditor::updateCurrentName(const QString& a_name)
    {
        setCurrentItem(a_name);

        // Validate name
        if (current_item.isEmpty() || current_item.back() == " ")
        {
            name_validation_label.setText("Invalid");
            item_valid = false;
        }
        else if (existing_items.contains(current_item))
        {
            name_validation_label.setText("Already exists; will be overwritten");
            item_valid = true;
        }
        else
        {
            name_validation_label.setText("Available");
            item_valid = true;
        }

        return;
    }
    
    QMessageBox::StandardButton AbstractEditor::promptOverwrite()
    {
        QMessageBox::StandardButton response = QMessageBox::Yes;

        if (existing_items.contains(current_item))
        {
            response = QMessageBox::warning
            (
                this,
                overwrite_title,
                QString() % "Are you sure you want to overwrite \"" % current_item % "\"?\nThis cannot be undone.",
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::Yes
            );
        }

        return response;
    }
    
    void AbstractEditor::createNameGroup()
    {
        connect(&name_edit, &QLineEdit::textChanged, this, &AbstractEditor::updateCurrentName);

        name_validator.setRegExp(name_regex);
        name_edit.setValidator(&name_validator);
        name_edit.setMaxLength(32);

        int layout_row = 0;

        name_group_layout.addWidget(&name_label, layout_row, 0);
        name_group_layout.addWidget(&name_edit, layout_row++, 1);
        name_group_layout.addWidget(&name_validation_label, layout_row++, 1, Qt::AlignRight);

        name_group.setLayout(&name_group_layout);
        return;
    }
    
    const QString AbstractEditor::getUniqueName(const QString& a_name)
    {
        QString unique_name = a_name;
        int index = 1;

        while (existing_items.contains(unique_name))
        {
            unique_name = a_name % "_" % QString::number(index++);
        }

        return unique_name;
    }
}