#ifndef __ABSTRACTEDITORLAUNCHER_H__
#define __ABSTRACTEDITORLAUNCHER_H__

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

#include "AbstractEditor.hpp"
#include "Language.hpp"
#include "Settings.hpp"

#include "EditorMode.hpp"

namespace wf
{
    class AbstractEditorLauncher : public QDialog
    {
        Q_OBJECT

        public:
            virtual ~AbstractEditorLauncher();

        public slots:
            void open();

        signals:
            void launchEditor(EditorMode a_mode, QString a_item);

        protected:
            AbstractEditorLauncher(
                AbstractEditor* a_editor,
                const QString& a_editor_subject,
                Settings* a_settings,
                QWidget* a_parent = nullptr);

            virtual bool exportSelected() = 0;
            virtual void deleteSelected() = 0;
            virtual void updateButtonState() = 0;

            Settings* settings;
            QComboBox dropdown;
            QFileDialog export_destination_dialog;
            QDialogButtonBox buttons;

            QButtonGroup radio_buttons;
            QRadioButton create_new_button{"Create new"};
            QRadioButton open_copy_button{"Open copy of existing"};
            QRadioButton edit_existing_button{"Edit existing"};
            QRadioButton export_existing_button{"Export existing"};
            QRadioButton delete_existing_button{"Delete existing"};
        
        protected slots:
            virtual void repopulateDropdown() = 0;
            void updateInterfaceState();

        private slots:
            void launch();
        
        private:
            void initialiseRadioButtons();
            void updateButtonText();

            QGridLayout grid_layout;
    };
}

#endif // __ABSTRACTEDITORLAUNCHER_H__