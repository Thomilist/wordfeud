#include "RecordDialog.hpp"


namespace wf
{
    RecordDialog::RecordDialog(RecordTracker* a_record_tracker, QWidget* a_parent)
        : QDialog(a_parent)
        , record_tracker(a_record_tracker)
        , buttons(QDialogButtonBox::Ok)
    {
        setModal(true);
        setWindowTitle("Records");
        connect(&buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);

        player_table = new QTableWidget();
        player_ai_table = new QTableWidget();
        combined_table = new QTableWidget();

        tab_widget.addTab(player_table, "Player");
        tab_widget.addTab(player_ai_table, "AI");
        tab_widget.addTab(combined_table, "Combined");

        player_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        player_ai_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        combined_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

        int row = 0;

        grid_layout.addWidget(&tab_widget, row++, 0);
        grid_layout.addWidget(&buttons, row++, 0);

        setLayout(&grid_layout);

        int width = std::max(combined_table->width(), std::max(player_table->width(), player_ai_table->width()));
        setMinimumWidth(width);
        setMinimumHeight(width);
    }
    
    RecordDialog::~RecordDialog()
    { }
    
    int RecordDialog::exec()
    {
        createRecordTable(player_table, record_tracker->getScores(PlayerType::Human));
        createRecordTable(player_ai_table, record_tracker->getScores(PlayerType::AI));
        createRecordTable(combined_table, combineRecords());

        

        return QDialog::exec();
    }
    
    void RecordDialog::createRecordTable(QTableWidget* a_table, std::vector<Score> a_records)
    {
        // Collumns: position, name, timestamp, points, result
        a_table->setColumnCount(4);
        a_table->setRowCount(a_records.size());
        a_table->setHorizontalHeaderLabels({"Name", "Date", "Score", "Result"});

        int row = 0;
        int collumn = 0;

        for (auto record : a_records)
        {
            collumn = 0;

            // Collumn 0: name
            QTableWidgetItem* name_item = new QTableWidgetItem(record.name);
            a_table->setItem(row, collumn++, name_item);

            // Collumn 1: timestamp
            QTableWidgetItem* timestamp_item = new QTableWidgetItem(record.timestamp);
            timestamp_item->setTextAlignment(Qt::AlignCenter);
            a_table->setItem(row, collumn++, timestamp_item);

            // Collumn 2: points
            QTableWidgetItem* points_item = new QTableWidgetItem(QString::number(record.points));
            points_item->setTextAlignment(Qt::AlignCenter);
            a_table->setItem(row, collumn++, points_item);

            // Collumn 3: result
            QTableWidgetItem* result_item = new QTableWidgetItem(record.result);
            result_item->setTextAlignment(Qt::AlignCenter);
            a_table->setItem(row, collumn++, result_item);

            ++row;
        }

        a_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        return;
    }
    
    std::vector<Score> RecordDialog::combineRecords()
    {
        std::vector<Score> player_records = record_tracker->getScores(PlayerType::Human);
        std::vector<Score> player_ai_records = record_tracker->getScores(PlayerType::AI);
        std::vector<Score> combined_records;

        std::copy(player_records.begin(), player_records.end(), std::back_inserter(combined_records));
        std::copy(player_ai_records.begin(), player_ai_records.end(), std::back_inserter(combined_records));
        std::sort(combined_records.begin(), combined_records.end(), RecordTracker::sortScoreByPoints);

        return combined_records;
    }
}