#ifndef __DICTIONARYLOOKUPDIALOG_H__
#define __DICTIONARYLOOKUPDIALOG_H__

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "Language.hpp"
#include "Settings.hpp"

namespace wf
{
    class DictionaryLookupDialog : public QDialog
    {
        Q_OBJECT

        public:
            DictionaryLookupDialog(Settings* a_settings, QWidget* a_parent = nullptr);
            ~DictionaryLookupDialog();

        public slots:
            void open();
            void updateStatusDisplay();
            void changeLanguage();

        private:
            void createLanguageDropdown(int a_row);

            Settings* settings;
            Language* lookup_language;
            QGridLayout grid_layout;
            QDialogButtonBox buttons;
            int max_word_length;

            QLabel word_input_label{"Word:"};
            QLabel word_status_label;
            QLineEdit word_input;
            QLabel language_label{"Dictionary:"};
            QComboBox language_dropdown;
    };
}

#endif // __DICTIONARYLOOKUPDIALOG_H__