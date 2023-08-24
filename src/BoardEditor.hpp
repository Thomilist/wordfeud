#ifndef __BOARDEDITOR_H__
#define __BOARDEDITOR_H__

#include "ForwardDeclarations.hpp"

#include "AbstractEditor.hpp"

namespace wf
{
    class BoardEditor : public AbstractEditor
    {
        public:
            BoardEditor(Settings* a_settings, QWidget* a_parent = nullptr);
            ~BoardEditor();
        
        private:
            void prepare() override;
            void save() override;
    };
}

#endif // __BOARDEDITOR_H__