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
        , biased_word_source_dialog(
            this,
            "Select biased words source",
            "resources/dictionaries",
            "Plain text (*.txt)"
        )
        , letter_editor(settings, this)
        , letter_table_model(&language)
        , button_box(QDialogButtonBox::Help | QDialogButtonBox::Save | QDialogButtonBox::Discard)
    {
        setModal(true);
        setLayout(&grid_layout);
        layout()->setSizeConstraint(QLayout::SetFixedSize);
        setWindowTitle("Dictionary Editor");

        button_box.button(QDialogButtonBox::Save)->setText("Save && Close");

        connect(&button_box, &QDialogButtonBox::accepted, this, &DictionaryEditor::saveAndClose);
        connect(button_box.button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &QDialog::reject);
        connect(&button_box, &QDialogButtonBox::helpRequested, this, &DictionaryEditor::viewEditorHelp);

        letter_table.setModel(&letter_table_model);

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
        source_language = a_language;
        language.setName(a_language);
        name_edit.setText(a_language);

        word_source_edit.clear();
        biased_word_source_edit.clear();

        if (a_mode == DictionaryEditorMode::OpenCopy || a_mode == DictionaryEditorMode::EditExisting)
        {
            loadExistingLanguage();
        }

        resizeTable();
        populateEditor();
        repaint();
        
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

        updateLetterPoolInfo();

        bool word_source_specified = !word_source_edit.text().isEmpty();

        button_box.button(QDialogButtonBox::Save)->setDisabled(!dictionary_name_valid || !word_source_specified);

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
    
    void DictionaryEditor::browseBiasedWordSource()
    {
        if (biased_word_source_dialog.exec())
        {
            QStringList files = biased_word_source_dialog.selectedFiles();
            biased_word_source_edit.setText(files.front());
        }

        return;
    }
    
    void DictionaryEditor::saveAndClose()
    {
        if (dictionaries.contains(language.getName()))
        {
            QMessageBox::StandardButton response = QMessageBox::Yes;

            response = QMessageBox::warning
            (
                this,
                "Overwrite dictionary",
                QString() % "Are you sure you want to overwrite \"" % language.getName() % "\"?\nThis cannot be undone.",
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::Yes
            );

            if (response == QMessageBox::No)
            {
                return;
            }
        }
        
        language.loadWordListFromFilePlain(word_source_edit.text());
        language.exportWordList(Language::getWordListPath(language.getName()));

        if (!biased_word_source_edit.text().isEmpty())
        {
            language.loadWordListFromFilePlain(biased_word_source_edit.text(), true);
            language.exportWordList(Language::getBiasedWordListPath(language.getName()), true);
        }

        language.exportLetterList(Language::getLetterListPath(language.getName()));
        
        QDialog::accept();
        return;
    }
    
    void DictionaryEditor::removeSelectedLetters()
    {
        std::set<int, std::greater<int>> selected_columns;
        auto selection_model = letter_table.selectionModel();
        QModelIndexList selected_indices = selection_model->selectedIndexes();

        for (auto index : selected_indices)
        {
            selected_columns.insert(index.column());
        }

        for (auto index : selected_columns)
        {
            letter_table_model.removeLetter(index);
        }

        return;
    }
    
    void DictionaryEditor::scrollLetterPoolToEnd()
    {
        QScrollBar* scroll_bar = letter_table.horizontalScrollBar();
        scroll_bar->setValue(scroll_bar->maximum());
        return;
    }
    
    void DictionaryEditor::populateEditor()
    {
        detectExistingDictionaries();
        updateHeaderText();
        updateNameSuggestion();
        letter_editor.reset();

        updateInterfaceState();

        return;
    }
    
    void DictionaryEditor::detectExistingDictionaries()
    {
        dictionaries = settings->getAvailableLanguagesAsStrings();
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
        connect(&biased_word_source_browse_button, &QPushButton::clicked, this, &DictionaryEditor::browseBiasedWordSource);
        connect(&biased_word_source_clear_button, &QPushButton::clicked, this, [this](){biased_word_source_edit.clear();});
        connect(&word_source_edit, &QLineEdit::textChanged, this, &DictionaryEditor::updateInterfaceState);

        word_source_edit.setReadOnly(true);
        biased_word_source_edit.setReadOnly(true);

        word_group_layout.setColumnStretch(0, 1);
        word_group_layout.setColumnStretch(1, 0);
        word_group_layout.setColumnStretch(2, 0);
        
        int layout_row = 0;

        word_group_layout.addWidget(&word_source_label, layout_row++, 0);
        word_group_layout.addWidget(&word_source_edit, layout_row, 0, 1, 2);
        word_group_layout.addWidget(&word_source_browse_button, layout_row++, 2);

        word_group_layout.addWidget(&biased_word_source_label, layout_row++, 0);
        word_group_layout.addWidget(&biased_word_source_edit, layout_row, 0);
        word_group_layout.addWidget(&biased_word_source_clear_button, layout_row, 1);
        word_group_layout.addWidget(&biased_word_source_browse_button, layout_row++, 2);
        
        word_group.setLayout(&word_group_layout);
        return;
    }
    
    void DictionaryEditor::createLetterGroup()
    {
        connect(&letter_editor, &LetterEditorWidget::addToPool, &letter_table_model, &LetterTableModel::addLetter);
        connect(&letter_editor, &LetterEditorWidget::addToPool, this, &DictionaryEditor::scrollLetterPoolToEnd);
        connect(&letter_editor, &LetterEditorWidget::removeSelected, this, &DictionaryEditor::removeSelectedLetters);
        connect(&letter_table_model, &LetterTableModel::dataChanged, this, &DictionaryEditor::updateInterfaceState);
        
        letter_table.horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        letter_table.horizontalHeader()->setVisible(false);

        letter_pool_info_font.setPixelSize(18);
        letter_pool_info_label.setAlignment(Qt::AlignRight);
        letter_pool_info_label.setText
        (
            QString()
            % "Unique letters: \n"
            % "Normal letters: \n"
            % "Wildcard letters: \n"
            % "Total letters: \n"
            % "Total letter points: \n"
            % "Average letter points: \n"
        );
        letter_pool_info_label.setFont(letter_pool_info_font);
        letter_pool_info_values_label.setFont(letter_pool_info_font);
        
        int layout_row = 0;

        letter_group_layout.addWidget(&letter_editor, layout_row, 0, 1, 2, Qt::AlignCenter);
        letter_group_layout.addWidget(&letter_pool_info_label, layout_row, 2, Qt::AlignRight);
        letter_group_layout.addWidget(&letter_pool_info_values_label, layout_row++, 3, Qt::AlignLeft);
        letter_group_layout.addWidget(&letter_table, layout_row++, 0, 1, 4, Qt::AlignCenter);

        letter_group_layout.setColumnStretch(0, 1);
        letter_group_layout.setColumnStretch(1, 1);
        letter_group_layout.setColumnStretch(2, 1);
        letter_group_layout.setColumnStretch(3, 0);

        letter_group_layout.setColumnMinimumWidth(3, 80);

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
    
    void DictionaryEditor::loadExistingLanguage()
    {
        if (source_language.isEmpty())
        {
            return;
        }

        word_source_edit.setText(QFileInfo(Language::getWordListPath(source_language)).canonicalFilePath());

        if (QFile::exists(Language::getBiasedWordListPath(source_language)))
        {
            biased_word_source_edit.setText(QFileInfo(Language::getBiasedWordListPath(source_language)).canonicalFilePath());
        }

        Language source_letters;
        source_letters.loadLettersFromFile(Language::getLetterListPath(source_language));

        letter_table_model.setLetterList(source_letters.getLetterList());

        return;
    }
    
    void DictionaryEditor::resizeTable()
    {
        // Width (it's just wide, no magic here)
        letter_table.setMinimumWidth(800);
        letter_table.resizeColumnsToContents();

        // Height to fit the three rows
        int scroll_bar_height = letter_table.horizontalScrollBar()->height();
        int total_row_height = 0;

        for (int index = 0; index < letter_table.verticalHeader()->count(); ++index)
        {
            total_row_height += letter_table.verticalHeader()->sectionSize(index);
        }

        int total_height = scroll_bar_height + total_row_height;
        letter_table.setFixedHeight(total_height);

        return;
    }
    
    void DictionaryEditor::updateLetterPoolInfo()
    {
        std::set<QString> unique_letters;
        int normal_letters = 0;
        int wildcard_letters = 0;
        int total_letters = 0;
        int total_letter_points = 0;
        double average_letter_points = 0;

        for (const auto& letter : *language.getLetterList())
        {
            unique_letters.insert(letter.letter);
            
            if (letter.letter.isEmpty())
            {
                wildcard_letters += letter.count;
            }
            else
            {
                normal_letters += letter.count;
            }

            total_letters += letter.count;
            total_letter_points += letter.points * letter.count;
        }

        average_letter_points = total_letters == 0 ? 0 : static_cast<double>(total_letter_points) / total_letters;

        letter_pool_info_values_label.setText
        (
            QString()
            % QString::number(unique_letters.size()) % "\n"
            % QString::number(normal_letters) % "\n"
            % QString::number(wildcard_letters) % "\n"
            % QString::number(total_letters) % "\n"
            % QString::number(total_letter_points) % "\n"
            % QString::number(average_letter_points, 'f', 2) % "\n"
        );

        return;
    }
}