#include "DictionaryEditor.hpp"


namespace wf
{
    DictionaryEditor::DictionaryEditor(Settings* a_settings, QWidget* a_parent)
        : AbstractEditor("Dictionary", a_settings, a_parent)
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
    {
        setEditorTitle("Dictionary Editor");
        setRejectTitle("Discard dictionary");
        setOverwriteTitle("Overwrite dictionary");
        setHelpURL("https://github.com/Thomilist/wordfeud/wiki/Dictionary-Editor");
        
        letter_table.setModel(&letter_table_model);
        word_source_dialog.setFileMode(QFileDialog::FileMode::ExistingFile);

        createWordGroup();
        createLetterGroup();

        int layout_row = 0;
        auto layout = getBodyLayout();

        layout->addWidget(&word_group, layout_row++, 0);
        layout->addWidget(&letter_group, layout_row++, 0);
    }
    
    DictionaryEditor::~DictionaryEditor()
    { }
    
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
    
    void DictionaryEditor::updateLetterPoolInfo()
    {
        std::set<QString> unique_letters;
        std::set<LetterData> unique_entries;
        int normal_letters = 0;
        int wildcard_letters = 0;
        int total_letters = 0;
        int total_letter_points = 0;
        double average_letter_points = 0;

        for (const auto& entry : *language.getLetterList())
        {
            if (entry.letter.isEmpty())
            {
                wildcard_letters += entry.count;
            }
            else
            {
                normal_letters += entry.count;
                unique_letters.insert(entry.letter);
            }

            unique_entries.insert(entry);
            total_letters += entry.count;
            total_letter_points += entry.points * entry.count;
        }

        average_letter_points = total_letters == 0 ? 0 : static_cast<double>(total_letter_points) / total_letters;

        letter_pool_info_values_label.setText
        (
            QString()
            % QString::number(unique_entries.size()) % " / " % QString::number(language.getLetterList()->size()) % "\n"
            % QString::number(unique_letters.size()) % "\n"
            % QString::number(normal_letters) % " / " % QString::number(wildcard_letters) % " / " % QString::number(total_letters) % "\n"
            % QString::number(average_letter_points, 'f', 2) % " / " % QString::number(total_letter_points) % "\n"
        );

        return;
    }
    
    void DictionaryEditor::resizeLetterTable()
    {
        // Width (it's just wide, no magic here)
        letter_table.setMinimumWidth(800);
        letter_table.resizeColumnsToContents();

        // Height to fit the three rows
        letter_table.setFixedHeight
        (
            letter_table.verticalHeader()->length()
            + letter_table.horizontalScrollBar()->height()
        );
        letter_table.adjustSize();

        return;
    }
    
    void DictionaryEditor::updateSaveButtonState()
    {
        bool word_source_specified = !word_source_edit.text().isEmpty();
        bool letter_pool_empty = language.getLetterList()->empty();

        button_box.button(QDialogButtonBox::Save)->setDisabled(!item_valid || !word_source_specified || letter_pool_empty);
        return;
    }
    
    void DictionaryEditor::prepare()
    {
        word_source_edit.clear();
        biased_word_source_edit.clear();

        if (mode == EditorMode::OpenCopy || mode == EditorMode::EditExisting)
        {
            loadExistingLanguage();
        }

        resizeLetterTable();
        populate();

        return;
    }
    
    void DictionaryEditor::save()
    {
        language.loadWordListFromFilePlain(word_source_edit.text());
        language.exportWordList(Language::getWordListPath(current_item));

        if (!biased_word_source_edit.text().isEmpty())
        {
            language.loadWordListFromFilePlain(biased_word_source_edit.text(), true);
            language.exportWordList(Language::getBiasedWordListPath(current_item), true);
        }

        language.exportLetterList(Language::getLetterListPath(current_item));
        return;
    }
    
    void DictionaryEditor::createWordGroup()
    {
        connect(&word_source_browse_button, &QPushButton::clicked, this, &DictionaryEditor::browseWordSource);
        connect(&biased_word_source_browse_button, &QPushButton::clicked, this, &DictionaryEditor::browseBiasedWordSource);
        connect(&biased_word_source_clear_button, &QPushButton::clicked, this, [this](){biased_word_source_edit.clear();});
        connect(&word_source_edit, &QLineEdit::textChanged, this, &DictionaryEditor::updateSaveButtonState);

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
        connect(&letter_table_model, &LetterTableModel::dataChanged, this, &DictionaryEditor::updateLetterPoolInfo);
        connect(&letter_table_model, &LetterTableModel::dataChanged, this, &DictionaryEditor::updateSaveButtonState);
        
        letter_table.horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        letter_table.horizontalHeader()->setVisible(false);

        letter_pool_info_font.setPixelSize(16);
        letter_pool_info_label.setAlignment(Qt::AlignRight);
        letter_pool_info_label.setText
        (
            QString()
            % "Entries (unique / total): \n"
            % "Unique letters (ex. wildcards): \n"
            % "Tiles (normal / wildcard / total): \n"
            % "Points (average / total): \n"
        );
        letter_pool_info_label.setFont(letter_pool_info_font);
        letter_pool_info_values_label.setFont(letter_pool_info_font);
        
        int layout_row = 0;

        letter_group_layout.addWidget(&letter_editor, layout_row, 0, Qt::AlignCenter);
        letter_group_layout.addWidget(&letter_pool_info_label, layout_row, 1, Qt::AlignRight | Qt::AlignVCenter);
        letter_group_layout.addWidget(&letter_pool_info_values_label, layout_row++, 2, Qt::AlignLeft | Qt::AlignVCenter);
        letter_group_layout.addWidget(&letter_table, layout_row++, 0, 1, 3, Qt::AlignCenter);

        letter_group_layout.setColumnStretch(0, 2);
        letter_group_layout.setColumnStretch(1, 1);
        letter_group_layout.setColumnStretch(2, 0);

        letter_group_layout.setColumnMinimumWidth(2, 120);

        letter_group.setLayout(&letter_group_layout);
        return;
    }
    
    void DictionaryEditor::loadExistingLanguage()
    {
        if (original_item.isEmpty())
        {
            return;
        }

        word_source_edit.setText(QFileInfo(Language::getWordListPath(original_item)).canonicalFilePath());

        if (QFile::exists(Language::getBiasedWordListPath(original_item)))
        {
            biased_word_source_edit.setText(QFileInfo(Language::getBiasedWordListPath(original_item)).canonicalFilePath());
        }

        Language source_letters;
        source_letters.loadLettersFromFile(Language::getLetterListPath(original_item));

        letter_table_model.setLetterList(source_letters.getLetterList());

        return;
    }
    
    void DictionaryEditor::populate()
    {
        setExistingItems(settings->getAvailableLanguagesAsStrings());
        updateHeaderText();
        updateNameSuggestion();
        letter_editor.reset();

        updateLetterPoolInfo();
        updateSaveButtonState();

        return;
    }
}