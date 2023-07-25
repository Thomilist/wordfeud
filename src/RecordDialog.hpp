#ifndef __RECORDDIALOG_H__
#define __RECORDDIALOG_H__

#include <limits>

#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QSizePolicy>
#include <QSpinBox>
#include <QTableView>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "RecordSortFilterProxyModel.hpp"
#include "RecordTableModel.hpp"

#include "PlayerType.hpp"

namespace wf
{
    class RecordDialog : public QDialog
    {
        Q_OBJECT
        
        public:
            RecordDialog(RecordTableModel* a_record_table_model, QWidget* a_parent = nullptr);
            ~RecordDialog();

        public slots:
            void open();
            void setRecordSource(RecordTableModel* a_record_table_model);
            void updateMinimumSize();
            void updatePointsFilterStatus();
            void updatePointsLimits();
            void resetFilters();
        
        signals:
            void pointsFilterStatusChanged(
                bool a_minimum_points_enabled,
                bool a_maximum_points_enabled,
                bool a_minimum_opponent_points_enabled,
                bool a_maximum_opponent_points_enabled);
            void pointsFilterValuesChanged(
                int a_minimum_points,
                int a_maximum_points,
                int a_minimum_opponent_points,
                int a_maximum_opponent_points);
        
        private:
            void initialisePointsLimits(int& a_layour_row);
            
            QGridLayout grid_layout;
            QTableView record_table;
            RecordSortFilterProxyModel record_proxy;
            RecordTableModel* record_table_model;
            QDialogButtonBox buttons;
            std::pair<int, int> int_limits = std::pair{std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};

            // Filters
            QScrollArea filter_scroll_area;
            QGroupBox filter_group{"Filters"};
            QGridLayout filter_layout;
            QPushButton reset_filters_button{"Reset Filters"};

            QGroupBox points_group{"Points"};
            QGridLayout points_group_layout;
            QCheckBox maximum_points_checkbox;
            QLabel maximum_points_label{"Maximum:"};
            QSpinBox maximum_points_input;
            QCheckBox minimum_points_checkbox;
            QLabel minimum_points_label{"Minimum:"};
            QSpinBox minimum_points_input;

            QGroupBox opponent_points_group{"Points, opponent"};
            QGridLayout opponent_points_group_layout;
            QCheckBox maximum_opponent_points_checkbox;
            QLabel maximum_opponent_points_label{"Maximum:"};
            QSpinBox maximum_opponent_points_input;
            QCheckBox minimum_opponent_points_checkbox;
            QLabel minimum_opponent_points_label{"Minimum:"};
            QSpinBox minimum_opponent_points_input;
    };
}

#endif // __RECORDDIALOG_H__