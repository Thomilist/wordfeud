#ifndef __DICTIONARYEDITOR_H__
#define __DICTIONARYEDITOR_H__

#include <set>

#include <QFileDialog>
#include <QFont>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QModelIndex>
#include <QModelIndexList>
#include <QPushButton>
#include <QScrollBar>
#include <QString>
#include <QStringBuilder>
#include <QStringList>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "AbstractEditor.hpp"
#include "Language.hpp"
#include "LetterEditorWidget.hpp"
#include "LetterTable.hpp"
#include "LetterTableModel.hpp"
#include "Settings.hpp"

namespace wf
{
    class DictionaryEditor : public AbstractEditor
    {
        public:
            DictionaryEditor(Settings* a_settings, QWidget* a_parent = nullptr);
            ~DictionaryEditor();
        
        private slots:
            void browseWordSource();
            void browseBiasedWordSource();

            void removeSelectedLetters();
            void scrollLetterPoolToEnd();
            void updateLetterPoolInfo();
            void resizeLetterTable();

            void updateSaveButtonState();

        private:
            void prepare() override;
            void save() override;

            void createWordGroup();
            void createLetterGroup();

            void loadExistingLanguage();
            void populate();

            Language language;

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
    };
}

#endif // __DICTIONARYEDITOR_H__