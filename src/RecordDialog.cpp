#include "RecordDialog.hpp"


namespace wf
{
    RecordDialog::RecordDialog(RecordTracker* a_record_tracker, QWidget* a_parent)
        : QDialog(a_parent, QDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint)
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

        int layout_row = 0;

        grid_layout.addWidget(&tab_widget, layout_row++, 0);
        grid_layout.addWidget(&buttons, layout_row++, 0, Qt::AlignCenter);

        setLayout(&grid_layout);
    }
    
    RecordDialog::~RecordDialog()
    { }
    
    void RecordDialog::open()
    {
        createRecordTable(player_table, record_tracker->getScores(PlayerType::Human));
        createRecordTable(player_ai_table, record_tracker->getScores(PlayerType::AI));
        createRecordTable(combined_table, combineRecords());

        setMinimumWidth(805);
        setMinimumHeight(500);

        QDialog::open();
        return;
    }
    
    void RecordDialog::createRecordTable(QTableWidget* a_table, std::vector<Score> a_records)
    {
        // Collumns: position, name, timestamp, points, result
        a_table->setColumnCount(7);
        a_table->setRowCount(a_records.size());
        a_table->setHorizontalHeaderLabels({"Name", "Date", "Dictionary", "Modifiers", "Difficulty", "Score", "Result"});

        int row = 0;
        int column = 0;

        for (auto record : a_records)
        {
            column = 0;

            // Column 0: name
            QTableWidgetItem* name_item = new QTableWidgetItem(record.name);
            a_table->setItem(row, column++, name_item);

            // Column 1: timestamp
            QTableWidgetItem* timestamp_item = new QTableWidgetItem(record.timestamp);
            timestamp_item->setTextAlignment(Qt::AlignCenter);
            a_table->setItem(row, column++, timestamp_item);

            // Column 2: dictionary
            QTableWidgetItem* dictionary_item = new QTableWidgetItem(record.dictionary);
            dictionary_item->setTextAlignment(Qt::AlignCenter);
            a_table->setItem(row, column++, dictionary_item);

            // Column 3: modifiers
            QTableWidgetItem* modifier_item = new QTableWidgetItem(record.modifier_pattern);
            modifier_item->setTextAlignment(Qt::AlignCenter);
            a_table->setItem(row, column++, modifier_item);

            // Column 4: difficulty
            QString difficulty = "-";

            if (record.difficulty > 0)
            {
                difficulty = QString::number(record.difficulty * 10) + "%";
            }
            
            QTableWidgetItem* difficulty_item = new QTableWidgetItem(difficulty);
            difficulty_item->setTextAlignment(Qt::AlignCenter);
            a_table->setItem(row, column++, difficulty_item);

            // Column 5: points
            QTableWidgetItem* points_item = new QTableWidgetItem(QString::number(record.points));
            points_item->setTextAlignment(Qt::AlignCenter);
            a_table->setItem(row, column++, points_item);

            // Column 6: result
            QTableWidgetItem* result_item = new QTableWidgetItem(record.result);
            result_item->setTextAlignment(Qt::AlignCenter);
            a_table->setItem(row, column++, result_item);

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