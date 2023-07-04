#include "DictionaryLookupDialog.hpp"


namespace wf
{
    DictionaryLookupDialog::DictionaryLookupDialog(Settings* a_settings, QWidget* a_parent)
        : QDialog(a_parent, QDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint)
        , settings(a_settings)
        , buttons(QDialogButtonBox::Close)
    {
        setModal(true);
        setWindowTitle("Dictionary lookup");

        connect(&buttons, &QDialogButtonBox::rejected, this, &QDialog::accept);
        connect(&word_input, &QLineEdit::textChanged, this, &DictionaryLookupDialog::updateStatusDisplay);
        connect(&language_dropdown, &QComboBox::currentTextChanged, this, &DictionaryLookupDialog::changeLanguage);

        int board_width = settings->getBoardDimensions().width();
        int board_height = settings->getBoardDimensions().height();
        max_word_length = board_width > board_height ? board_width : board_height;

        word_input.setMaxLength(max_word_length);
        word_input.setFixedWidth(250);

        int layout_row = 0;

        grid_layout.addWidget(&word_input_label, layout_row, 0);
        grid_layout.addWidget(&word_status_label, layout_row++, 1, Qt::AlignRight);

        grid_layout.addWidget(&word_input, layout_row++, 0, 1, 2);
        
        createLanguageDropdown(layout_row++);

        grid_layout.addWidget(&buttons, layout_row++, 0, 1, 2, Qt::AlignCenter);
        
        setLayout(&grid_layout);
        updateStatusDisplay();
    }
    
    DictionaryLookupDialog::~DictionaryLookupDialog()
    { }
    
    void DictionaryLookupDialog::open()
    {
        QDialog::open();
        return;
    }
    
    void DictionaryLookupDialog::updateStatusDisplay()
    {
        QString word = word_input.text();
        bool is_valid = false;

        if (word.length() > 0)
        {
            is_valid = lookup_language->isInWordList(word.toUpper());
        }

        word_status_label.setText
        (
            QString()
            + (is_valid ? "Valid" : "Invalid")
            + ", "
            + QString::number(word.length())
            + "/"
            + QString::number(max_word_length)
        );
    }
    
    void DictionaryLookupDialog::changeLanguage()
    {
        lookup_language = settings->getLanguage(language_dropdown.currentText());
        updateStatusDisplay();
        return;
    }
    
    void DictionaryLookupDialog::createLanguageDropdown(int a_row)
    {
        grid_layout.addWidget(&language_label, a_row, 0);
        grid_layout.addWidget(&language_dropdown, a_row, 1);

        int index = 0;

        for (auto& language : settings->getAvailableLanguages())
        {
            language_dropdown.addItem(language.asString());

            if (language.asEnum() == settings->getTempLanguage()->asEnum())
            {
                language_dropdown.setCurrentIndex(index);
                lookup_language = &language;
            }

            ++index;
        }

        return;
    }
}