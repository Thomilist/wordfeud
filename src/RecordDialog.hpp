#ifndef __RECORDDIALOG_H__
#define __RECORDDIALOG_H__

#include <algorithm>
#include <vector>

#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGuiApplication>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTabWidget>

#include "ForwardDeclarations.hpp"

#include "RecordTracker.hpp"

#include "PlayerType.hpp"

namespace wf
{
    class RecordDialog : public QDialog
    {
        Q_OBJECT
        
        public:
            RecordDialog(RecordTracker* a_record_tracker, QWidget* a_parent);
            ~RecordDialog();

        public slots:
            void open();
        
        private:
            void createRecordTable(QTableWidget* a_table, std::vector<Score> a_records);
            std::vector<Score> combineRecords();

            QGridLayout grid_layout;
            RecordTracker* record_tracker;
            QDialogButtonBox buttons;
            QTabWidget tab_widget;
            QTableWidget* player_table;
            QTableWidget* player_ai_table;
            QTableWidget* combined_table;

    };
}

#endif // __RECORDDIALOG_H__