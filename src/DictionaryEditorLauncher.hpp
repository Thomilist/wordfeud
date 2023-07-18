#ifndef __DICTIONARYEDITORLAUNCHER_H__
#define __DICTIONARYEDITORLAUNCHER_H__

#include <QDebug>

#include <QAbstractButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QStringBuilder>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "DictionaryEditor.hpp"
#include "Language.hpp"
#include "Settings.hpp"

#include "DictionaryEditorMode.hpp"

namespace wf
{
    class DictionaryEditorLauncher : public QDialog
    {
        Q_OBJECT

        public:
            DictionaryEditorLauncher(Settings* a_settings, QWidget* a_parent);
            ~DictionaryEditorLauncher();

        public slots:
            void open();
            void launch();
            void repopulateDictionaryDropdown(QAbstractButton* a_button);
            void updateInteractState();

        signals:
            void launchDictionaryEditor(DictionaryEditorMode a_mode, QString a_language);
            void viewEditorHelp();
        
        private:
            void initialiseRadioButtons();
            void initaliseDictionaryDropdown();
            void deleteDictionary(QString a_language);

            Settings* settings;
            QGridLayout grid_layout;
            QDialogButtonBox buttons;

            QLabel choose_dictionary_label;

            QButtonGroup radio_buttons;
            QRadioButton create_new_button;
            QRadioButton open_copy_button;
            QRadioButton edit_existing_button;
            QRadioButton delete_existing_button;

            QComboBox dictionary_dropdown;
    };
}

#endif // __DICTIONARYEDITORLAUNCHER_H__