#include "LetterTable.hpp"


namespace wf
{
    LetterTable::LetterTable(QWidget* a_parent)
        : QTableView(a_parent)
    {
        setSelectionBehavior(QAbstractItemView::SelectColumns);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
        horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
    
    LetterTable::~LetterTable()
    { }
    
    void LetterTable::wheelEvent(QWheelEvent* a_event)
    {
        int angle_delta = a_event->angleDelta().y();
        QScrollBar* scroll_bar = horizontalScrollBar();
        int position = scroll_bar->value();

        // Down -> right
        // Up -> left
        if (angle_delta < 0 && position < scroll_bar->maximum())
        {
            ++position;
        }
        else if (angle_delta > 0 && position > scroll_bar->minimum())
        {
            --position;
        }

        scroll_bar->setValue(position);

        return;
    }
}