#ifndef __DICTIONARYEDITORLAUNCHER_H__
#define __DICTIONARYEDITORLAUNCHER_H__

#include <QDebug>

#include <QAbstractButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QStandardPaths>
#include <QString>
#include <QStringBuilder>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "DictionaryEditor.hpp"
#include "Language.hpp"
#include "Settings.hpp"

#include "EditorMode.hpp"

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
            void launchDictionaryEditor(EditorMode a_mode, QString a_language);
            void viewEditorHelp();
        
        private:
            void initialiseRadioButtons();
            void initaliseDictionaryDropdown();
            bool exportDictionary(QString a_language);
            void deleteDictionary(QString a_language);

            Settings* settings;
            QGridLayout grid_layout;
            QDialogButtonBox buttons;
            bool using_internal_languages = false;

            QLabel choose_dictionary_label{"Choose dictionary:"};

            QButtonGroup radio_buttons;
            QRadioButton create_new_button{"Create new"};
            QRadioButton open_copy_button{"Open copy of existing"};
            QRadioButton edit_existing_button{"Edit existing"};
            QRadioButton export_existing_button{"Export existing"};
            QRadioButton delete_existing_button{"Delete existing"};

            QComboBox dictionary_dropdown;

            QFileDialog export_destination_dialog;
    };
}

#endif // __DICTIONARYEDITORLAUNCHER_H__