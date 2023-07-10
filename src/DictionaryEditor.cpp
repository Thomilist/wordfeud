#include "DictionaryEditor.hpp"


namespace wf
{
    DictionaryEditor::DictionaryEditor(Settings* a_settings, QWidget* a_parent)
        : QDialog(a_parent, QDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint)
        , settings(a_settings)
        , word_source_dialog(
            this,
            "Select word list source",
            "resources/dictionaries",
            "Plain text (*.txt)"
        )
        , button_box(QDialogButtonBox::Help | QDialogButtonBox::Save | QDialogButtonBox::Discard)
    {
        setModal(true);
        setLayout(&grid_layout);
        setWindowTitle("Dictionary Editor");

        button_box.button(QDialogButtonBox::Save)->setText("Save && Close");

        connect(&button_box, &QDialogButtonBox::accepted, this, &DictionaryEditor::saveAndClose);
        connect(button_box.button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &QDialog::reject);
        connect(&button_box, &QDialogButtonBox::helpRequested, this, &DictionaryEditor::viewEditorHelp);

        word_source_dialog.setFileMode(QFileDialog::FileMode::ExistingFile);

        header_font.setPixelSize(30);

        name_validator.setRegExp(name_regex);
        name_edit.setValidator(&name_validator);
        name_edit.setMaxLength(32);

        createWordGroup();
        createLetterGroup();
        createNameGroup();

        int layout_row = 0;

        grid_layout.addWidget(&header, layout_row++, 0);
        grid_layout.addWidget(&word_group, layout_row++, 0);
        grid_layout.addWidget(&letter_group, layout_row++, 0);
        grid_layout.addWidget(&name_group, layout_row++, 0);
        grid_layout.addWidget(&button_box, layout_row++, 0, Qt::AlignCenter);
    }
    
    DictionaryEditor::~DictionaryEditor()
    { }
    
    void DictionaryEditor::reject()
    {
        QMessageBox::StandardButton response = QMessageBox::Yes;

        response = QMessageBox::warning
        (
            this,
            "Discard dictionary",
            "Any unsaved data will be lost. Continue?",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes
        );

        if (response == QMessageBox::Yes)
        {
            QDialog::reject();
        }

        return;
    }
    
    void DictionaryEditor::launch(DictionaryEditorMode a_mode, QString a_language)
    {
        mode = a_mode;
        language.setName(a_language);

        populateEditor();
        
        QDialog::open();
        return;
    }
    
    void DictionaryEditor::viewEditorHelp()
    {
        QDesktopServices::openUrl(QUrl{"https://github.com/Thomilist/wordfeud/wiki/Dictionary-Editor", QUrl::TolerantMode});
        return;
    }
    
    void DictionaryEditor::updateInterfaceState()
    {
        // Dictionary name validation
        language.setName(name_edit.text());

        if (language.getName().isEmpty() || language.getName().back() == " ")
        {
            name_validation_label.setText("Invalid");
            dictionary_name_valid = false;
        }
        else if (dictionaries.contains(language.getName()))
        {
            name_validation_label.setText("Already exists; will be overwritten");
            dictionary_name_valid = true;
        }
        else
        {
            name_validation_label.setText("Available");
            dictionary_name_valid = true;
        }

        button_box.button(QDialogButtonBox::Save)->setDisabled(!dictionary_name_valid);

        return;
    }
    
    void DictionaryEditor::browseWordSource()
    {
        if (word_source_dialog.exec())
        {
            QStringList files = word_source_dialog.selectedFiles();
            word_source_edit.setText(files.front());
        }
        
        return;
    }
    
    void DictionaryEditor::saveAndClose()
    {
        language.loadWordListFromFilePlain(word_source_edit.text());
        language.exportWordList(
                QString()
                % "resources/dictionaries/"
                % language.getName()
                % "/"
                % language.getName()
                % "-words.txt"
            );
        
        QDialog::accept();
        return;
    }
    
    void DictionaryEditor::populateEditor()
    {
        detectExistingDictionaries();
        updateHeaderText();
        updateNameSuggestion();

        updateInterfaceState();

        return;
    }
    
    void DictionaryEditor::detectExistingDictionaries()
    {
        dictionaries = settings->getAvailableLanguages();
        return;
    }
    
    void DictionaryEditor::updateHeaderText()
    {
        switch (mode)
        {
            case DictionaryEditorMode::CreateNew:
            {
                header.setText
                (
                    "Creating New Dictionary"
                );

                break;
            }
            case DictionaryEditorMode::OpenCopy:
            {
                header.setText
                (
                    QString()
                    % "Editing Copy of \""
                    % language.getName()
                    % "\""
                );

                break;
            }
            case DictionaryEditorMode::EditExisting:
            {
                header.setText
                (
                    QString()
                    % "Editing \""
                    % language.getName()
                    % "\""
                );

                break;
            }
            case DictionaryEditorMode::ExportExisting:
            {
                header.setText
                (
                    QString()
                    % "Exporting \""
                    % language.getName()
                    % "\""
                    % " - how are you seeing this?"
                );

                break;
            }
            case DictionaryEditorMode::DeleteExisting:
            {
                header.setText
                (
                    QString()
                    % "Deleting \""
                    % language.getName()
                    % "\""
                    % " - how are you seeing this?"
                );

                break;
            }
        }

        header.setFont(header_font);

        return;
    }
    
    void DictionaryEditor::updateNameSuggestion()
    {
        switch (mode)
        {
            case DictionaryEditorMode::CreateNew:
            {
                language.setName(getUniqueDictionaryName("New Dictionary"));
                break;
            }
            case DictionaryEditorMode::OpenCopy:
            {
                language.setName(getUniqueDictionaryName(language.getName() % "_Copy"));
                break;
            }
            case DictionaryEditorMode::EditExisting:
            {
                break;
            }
            case DictionaryEditorMode::ExportExisting:
            {
                break;
            }
            case DictionaryEditorMode::DeleteExisting:
            {
                break;
            }
        }

        name_edit.setText(language.getName());

        return;
    }
    
    QString DictionaryEditor::getUniqueDictionaryName(QString a_dictionary_name)
    {
        QString unique_name = a_dictionary_name;
        int index = 1;

        while (dictionaries.contains(unique_name))
        {
            unique_name = a_dictionary_name % "_" % QString::number(index++);
        }

        return unique_name;
    }
    
    void DictionaryEditor::createWordGroup()
    {
        connect(&word_source_browse_button, &QPushButton::clicked, this, &DictionaryEditor::browseWordSource);

        word_source_edit.setReadOnly(true);
        
        int layout_row = 0;

        word_group_layout.addWidget(&word_source_label, layout_row++, 0);
        word_group_layout.addWidget(&word_source_edit, layout_row, 0);
        word_group_layout.addWidget(&word_source_browse_button, layout_row++, 1);
        
        word_group.setLayout(&word_group_layout);
        return;
    }
    
    void DictionaryEditor::createLetterGroup()
    {
        letter_group.setLayout(&letter_group_layout);
        return;
    }
    
    void DictionaryEditor::createNameGroup()
    {
        connect(&name_edit, &QLineEdit::textChanged, this, &DictionaryEditor::updateInterfaceState);

        int layout_row = 0;

        name_group_layout.addWidget(&name_label, layout_row, 0);
        name_group_layout.addWidget(&name_edit, layout_row++, 1);
        name_group_layout.addWidget(&name_validation_label, layout_row++, 1, Qt::AlignRight);

        name_group.setLayout(&name_group_layout);
        return;
    }
}