#ifndef __DICTIONARYEDITORLAUNCHER_H__
#define __DICTIONARYEDITORLAUNCHER_H__

#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QStringBuilder>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "AbstractEditorLauncher.hpp"
#include "Language.hpp"
#include "Settings.hpp"

#include "EditorMode.hpp"

namespace wf
{
    class DictionaryEditorLauncher : public AbstractEditorLauncher
    {
        Q_OBJECT

        public:
            DictionaryEditorLauncher(
                AbstractEditor* a_editor,
                const QString& a_editor_subject,
                Settings* a_settings,
                QWidget* a_parent = nullptr);
            ~DictionaryEditorLauncher();
        
        private:
            void repopulateDropdown() override;
            bool exportSelected() override;
            void deleteSelected() override;
            void updateButtonState() override;

            bool using_internal_languages = false;
    };
}

#endif // __DICTIONARYEDITORLAUNCHER_H__