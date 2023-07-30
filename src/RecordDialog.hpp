#ifndef __RECORDDIALOG_H__
#define __RECORDDIALOG_H__

#include <array>
#include <limits>
#include <set>

#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
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
            void updateControlFilter();
            void updateOpponentControlFilter();
            void updateDictionaryFilter();
            void updateModifierFilter();
            void updatePointsFilterStatus();
            void updatePointsLimits();
            void updateLastPointsLimits();
            void resetFilters();
            void repopulateFilters();
            void prepareView();
            void updateRecordTableWidth();
        
        signals:
            void controlFilterChanged(std::set<QString, ScoreControlCompare> a_control_list);
            void opponentControlFilterChanged(std::set<QString, ScoreControlCompare> a_opponent_control_list);
            void dictionaryFilterChanged(std::set<QString> a_dictionary_list);
            void modifierFilterChanged(std::set<QString> a_modifier_list);
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
            void fitListToContents(QListWidget* a_list);
            void fitListToContents(QListWidget* a_first_list, QListWidget* a_second_list);
            void fitTableWidthToContents(QTableView* a_table);
            void initialiseRecordTable();
            void initialiseControlList(int& a_layout_row);
            void populateControlList();
            void initialiseOpponentControlList(int& a_layout_row);
            void populateOpponentControlList();
            void initialiseDictionaryList(int& a_layout_row);
            void populateDictionaryList();
            void initialiseModifierList(int& a_layout_row);
            void populateModifierList();
            void initialisePointsLimits(int& a_layout_row);
            
            QGridLayout grid_layout;
            QTableView record_table;
            RecordSortFilterProxyModel record_proxy;
            RecordTableModel* record_table_model;
            QDialogButtonBox buttons;
            std::pair<int, int> int_limits = std::pair{std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};
            const int half_filter_width = 200;
            const int full_filter_width = 420;
            const int table_height = 750;

            // General filter elements
            QGridLayout filter_layout;
            QPushButton reset_filters_button{"Reset All Filters"};
            QWidget filter_padding;

            // Control filter
            QGroupBox control_group{"Control"};
            QGridLayout control_layout;
            QListWidget control_list;

            // Opponent control filter
            QGroupBox opponent_control_group{"Opponent Control"};
            QGridLayout opponent_control_layout;
            QListWidget opponent_control_list;

            // Name filter
            QGroupBox name_group{"Name"};
            QGridLayout name_layout;
            QLineEdit name_input;

            // Opponent name filter
            QGroupBox opponent_name_group{"Opponent Name"};
            QGridLayout opponent_name_layout;
            QLineEdit opponent_name_input;

            // Dictionary filter
            QGroupBox dictionary_group{"Dictionary"};
            QGridLayout dictionary_layout;
            QListWidget dictionary_list;

            // Modifier filter
            QGroupBox modifier_group{"Modifiers"};
            QGridLayout modifier_layout;
            QListWidget modifier_list;

            // Points filter
            QGroupBox points_group{"Points"};
            QGridLayout points_group_layout;
            QCheckBox maximum_points_checkbox;
            QLabel maximum_points_label{"Max:"};
            QSpinBox maximum_points_input;
            QCheckBox minimum_points_checkbox;
            QLabel minimum_points_label{"Min:"};
            QSpinBox minimum_points_input;

            // Minimum, maximum, minimum opponent, maximum opponent
            std::array<int, 4> last_points_filter_values{0,0,0,0};

            // Opponent points filter
            QGroupBox opponent_points_group{"Opponent Points"};
            QGridLayout opponent_points_group_layout;
            QCheckBox maximum_opponent_points_checkbox;
            QLabel maximum_opponent_points_label{"Max:"};
            QSpinBox maximum_opponent_points_input;
            QCheckBox minimum_opponent_points_checkbox;
            QLabel minimum_opponent_points_label{"Min:"};
            QSpinBox minimum_opponent_points_input;
    };
}

#endif // __RECORDDIALOG_H__