#ifndef __DICTIONARYEDITOR_H__
#define __DICTIONARYEDITOR_H__

#include <set>
#include <vector>

#include <QButtonGroup>
#include <QComboBox>
#include <QDesktopServices>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QModelIndex>
#include <QModelIndexList>
#include <QPushButton>
#include <QRadioButton>
#include <QRegExp>
#include <QRegExpValidator>
#include <QScrollBar>
#include <QSizePolicy>
#include <QString>
#include <QStringBuilder>
#include <QTableView>
#include <QUrl>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "LetterEditorWidget.hpp"
#include "LetterTable.hpp"
#include "LetterTableModel.hpp"
#include "Settings.hpp"

#include "DictionaryEditorMode.hpp"

namespace wf
{
    class DictionaryEditor : public QDialog
    {
        Q_OBJECT

        public:
            DictionaryEditor(Settings* a_settings, QWidget* a_parent = nullptr);
            ~DictionaryEditor();
        
        public slots:
            void reject();
            void launch(DictionaryEditorMode a_mode, QString a_language);
            void viewEditorHelp();
            void updateInterfaceState();
            void browseWordSource();
            void browseBiasedWordSource();
            void saveAndClose();
            void removeSelectedLetters();
            void scrollLetterPoolToEnd();
        
        private:
            void populateEditor();
            void detectExistingDictionaries();
            void updateHeaderText();
            void updateNameSuggestion();
            QString getUniqueDictionaryName(QString a_language);
            void createWordGroup();
            void createLetterGroup();
            void createNameGroup();
            void loadExistingLanguage();
            void resizeTable();
            void updateLetterPoolInfo();

            Settings* settings;
            QGridLayout grid_layout;
            Language language;

            // Launch parameters
            DictionaryEditorMode mode;

            // Existing dictionaries
            std::set<QString> dictionaries;
            QString source_language;

            // Dictionary metadata
            bool dictionary_name_valid = false;

            // UI elements

            // Header elements
            QLabel header;
            QFont header_font;

            // Word group elements
            QGroupBox word_group{"Word List"};
            QGridLayout word_group_layout;
            QLabel word_source_label{"Word list source:"};
            QLineEdit word_source_edit;
            QPushButton word_source_browse_button{"..."};
            QFileDialog word_source_dialog;
            QLabel biased_word_source_label{"Biased words source (optional):"};
            QLineEdit biased_word_source_edit;
            QPushButton biased_word_source_clear_button{"Clear"};
            QPushButton biased_word_source_browse_button{"..."};
            QFileDialog biased_word_source_dialog;

            // Letter group elements
            QGroupBox letter_group{"Letter Pool"};
            QGridLayout letter_group_layout;
            LetterEditorWidget letter_editor;
            LetterTable letter_table;
            LetterTableModel letter_table_model;
            QLabel letter_pool_info_label;
            QLabel letter_pool_info_values_label;
            QFont letter_pool_info_font;

            // Name group elements
            QGroupBox name_group{"Dictionary Name"};
            QGridLayout name_group_layout;
            QLabel name_label{"Name:"};
            QLineEdit name_edit;
            QLabel name_validation_label;
            QRegExpValidator name_validator;
            QRegExp name_regex{"^[a-zA-Z0-9](?:[a-zA-Z0-9 ._-]*[a-zA-Z0-9])?\\.[a-zA-Z0-9_-]+$"};

            // Lower bar elements
            QDialogButtonBox button_box;
    }; 
}

#endif // __DICTIONARYEDITOR_H__