#ifndef __ABSTRACTEDITOR_H__
#define __ABSTRACTEDITOR_H__

#include <set>

#include <QDesktopServices>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFont>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRegExp>
#include <QRegExpValidator>
#include <QString>
#include <QStringBuilder>
#include <QUrl>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "Settings.hpp"

#include "EditorMode.hpp"

namespace wf
{
    class AbstractEditor : public QDialog
    {
        Q_OBJECT
        
        public:
            virtual ~AbstractEditor() = default;
        
        public slots:
            void launch(EditorMode a_mode, QString a_name);
            void viewEditorHelp();
        
        protected:
            AbstractEditor(
                const QString& a_editor_subject,
                Settings* a_settings,
                QWidget* a_parent = nullptr);

            virtual void prepare() = 0;
            virtual void save() = 0;

            QGridLayout* getBodyLayout();

            void setEditorTitle(const QString& a_title);
            void setRejectTitle(const QString& a_title);
            void setRejectBody(const QString& a_body);
            void setOverwriteTitle(const QString& a_title);
            void setHelpURL(const QString& a_url);
            void setExistingItems(const std::set<QString>& a_items);
            void setCurrentItem(const QString& a_item);

            void updateHeaderText();
            void updateNameSuggestion();

            Settings* settings;
            QDialogButtonBox button_box;
            EditorMode mode;
            std::set<QString> existing_items;
            QString original_item;
            QString current_item;
            bool item_valid = false;

        private slots:
            void reject();
            void saveAndClose();
            void updateCurrentName(const QString& a_name);

        private:
            QMessageBox::StandardButton promptOverwrite();
            void createNameGroup();
            const QString getUniqueName(const QString& a_name);

            QGridLayout grid_layout;
            QString editor_subject;
            QString reject_title{"Discard"};
            QString reject_body{"Any unsaved data will be lost. Continue?"};
            QUrl help_url{"https://github.com/Thomilist/wordfeud/wiki"};
            QString overwrite_title{"Overwrite"};

            // Header elements
            QLabel header;
            QFont header_font;

            // Editor body
            QGridLayout body_layout;

            // Name group elements
            QGroupBox name_group{"Save As"};
            QGridLayout name_group_layout;
            QLabel name_label{"Name:"};
            QLineEdit name_edit;
            QLabel name_validation_label;
            QRegExpValidator name_validator;
            QRegExp name_regex{"^[a-zA-Z0-9](?:[a-zA-Z0-9 ._-]*[a-zA-Z0-9])?\\.[a-zA-Z0-9_-]+$"};
    };
}

#endif // __ABSTRACTEDITOR_H__