#ifndef __LETTERTABLE_H__
#define __LETTERTABLE_H__

#include <QHeaderView>
#include <QScrollBar>
#include <QTableView>
#include <QWheelEvent>
#include <QWidget>

#include "ForwardDeclarations.hpp"

namespace wf
{
    class LetterTable : public QTableView
    {
        Q_OBJECT

        public:
            LetterTable(QWidget* a_parent = nullptr);
            ~LetterTable();
        
        private:
            void wheelEvent(QWheelEvent* a_event) override;
    };
}

#endif // __LETTERTABLE_H__