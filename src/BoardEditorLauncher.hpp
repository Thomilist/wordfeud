#ifndef __BOARDEDITORLAUNCHER_H__
#define __BOARDEDITORLAUNCHER_H__

#include <QString>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "AbstractEditorLauncher.hpp"
#include "Settings.hpp"

namespace wf
{
    class BoardEditorLauncher : public AbstractEditorLauncher
    {
        public:
            BoardEditorLauncher(
                AbstractEditor* a_editor,
                Settings* a_settings,
                QWidget* a_parent = nullptr);
            ~BoardEditorLauncher();
        
        private:
            bool exportSelected() override;
            void deleteSelected() override;
            void updateButtonState() override;
            void repopulateDropdown() override;
    };
}

#endif // __BOARDEDITORLAUNCHER_H__