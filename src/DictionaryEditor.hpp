#ifndef __DICTIONARYEDITOR_H__
#define __DICTIONARYEDITOR_H__

#include <set>
#include <vector>

#include <QDebug>

#include <QButtonGroup>
#include <QComboBox>
#include <QDesktopServices>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
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
#include <QString>
#include <QStringBuilder>
#include <QTableView>
#include <QUrl>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "LetterEditorWidget.hpp"
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
            void saveAndClose();
            void removeSelectedLetters();
        
        signals:
            void letterPoolChanged(const QModelIndex& a_index = QModelIndex());
        
        private:
            void populateEditor();
            void detectExistingDictionaries();
            void updateHeaderText();
            void updateNameSuggestion();
            QString getUniqueDictionaryName(QString a_language);
            void createWordGroup();
            void createLetterGroup();
            void createNameGroup();
            void loadLettersFromSource();
            void resizeTable();

            Settings* settings;
            QGridLayout grid_layout;
            Language language;

            // Launch parameters
            DictionaryEditorMode mode;

            // Existing dictionaries
            std::set<QString> dictionaries;
            Language* source_language = nullptr;

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

            // Letter group elements
            QGroupBox letter_group{"Letter Pool"};
            QGridLayout letter_group_layout;
            LetterEditorWidget letter_editor;
            QTableView letter_table;
            LetterTableModel letter_table_model;

            // Name group elements
            QGroupBox name_group{"Dictionary Name"};
            QGridLayout name_group_layout;
            QLabel name_label{"Name"};
            QLineEdit name_edit;
            QLabel name_validation_label;
            QRegExpValidator name_validator;
            QRegExp name_regex{"^[a-zA-Z0-9](?:[a-zA-Z0-9 ._-]*[a-zA-Z0-9])?\\.[a-zA-Z0-9_-]+$"};

            // Lower bar elements
            QDialogButtonBox button_box;
    }; 
}

#endif // __DICTIONARYEDITOR_H__