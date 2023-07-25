#include "RecordDialog.hpp"


namespace wf
{
    RecordDialog::RecordDialog(RecordTableModel* a_record_table_model, QWidget* a_parent)
        : QDialog(a_parent, QDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint)
        , record_proxy(this)
        , buttons(QDialogButtonBox::Close)
    {
        setModal(true);
        setWindowTitle("Records");
        connect(&buttons, &QDialogButtonBox::rejected, this, &QDialog::accept);

        setRecordSource(a_record_table_model);
        record_table.setModel(&record_proxy);
        record_table.setSelectionMode(QAbstractItemView::NoSelection);
        record_table.setEditTriggers(QAbstractItemView::NoEditTriggers);
        record_table.horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        record_table.verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        connect(this, &RecordDialog::pointsFilterValuesChanged, &record_proxy, &RecordSortFilterProxyModel::updatePointsFilterValues);
        connect(this, &RecordDialog::pointsFilterStatusChanged, &record_proxy, &RecordSortFilterProxyModel::enablePointsFilters);
        connect(&record_proxy, &RecordSortFilterProxyModel::rowsRemoved, this, &RecordDialog::updateMinimumSize);
        connect(&record_proxy, &RecordSortFilterProxyModel::rowsInserted, this, &RecordDialog::updateMinimumSize);

        reset_filters_button.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        connect(&reset_filters_button, &QPushButton::clicked, this, &RecordDialog::resetFilters);

        int filter_layout_row = 0;

        initialisePointsLimits(filter_layout_row);
        filter_layout.addWidget(&reset_filters_button, filter_layout_row, 0, 1, 3, Qt::AlignHCenter | Qt::AlignBottom);

        filter_group.setLayout(&filter_layout);
        filter_scroll_area.setWidget(&filter_group);
        
        int layout_row = 0;

        grid_layout.addWidget(&filter_scroll_area, layout_row, 0);
        grid_layout.addWidget(&record_table, layout_row++, 1);
        grid_layout.addWidget(&buttons, layout_row++, 0, 1, 2, Qt::AlignCenter);

        grid_layout.setColumnStretch(0, 0);
        grid_layout.setColumnStretch(1, 1);

        setLayout(&grid_layout);
    }
    
    RecordDialog::~RecordDialog()
    { }
    
    void RecordDialog::open()
    {
        updateMinimumSize();
        QDialog::open();
        return;
    }
    
    void RecordDialog::setRecordSource(RecordTableModel* a_record_table_model)
    {
        record_table_model = a_record_table_model;
        record_proxy.setSourceModel(record_table_model);
        connect(record_table_model, &QAbstractItemModel::rowsInserted, this, &RecordDialog::updateMinimumSize);
        connect(record_table_model, &RecordTableModel::pointsLimitsChanged, this, &RecordDialog::updatePointsLimits);
        return;
    }
    
    void RecordDialog::updateMinimumSize()
    {
        setMinimumHeight(500);
        setMinimumWidth
        (
            26
            + filter_group.width()
            + record_table.verticalHeader()->width()
            + record_table.horizontalHeader()->length()
            + record_table.verticalScrollBar()->width()
        );
        adjustSize();

        return;
    }
    
    void RecordDialog::updatePointsFilterStatus()
    {
        for (auto [checkbox, input] :{
            std::pair{&minimum_points_checkbox, &minimum_points_input},
            std::pair{&maximum_points_checkbox, &maximum_points_input},
            std::pair{&minimum_opponent_points_checkbox, &minimum_opponent_points_input},
            std::pair{&maximum_opponent_points_checkbox, &maximum_opponent_points_input}
        })
        {
            input->setEnabled(checkbox->isChecked());
        }
        
        emit pointsFilterStatusChanged(
            minimum_points_checkbox.isChecked(),
            maximum_points_checkbox.isChecked(),
            minimum_opponent_points_checkbox.isChecked(),
            maximum_opponent_points_checkbox.isChecked());

        return;
    }
    
    void RecordDialog::updatePointsLimits()
    {
        for (auto [min_input, max_input, is_opponent] :
        {
            std::tuple{&minimum_points_input, &maximum_points_input, false},
            std::tuple{&minimum_opponent_points_input, &maximum_opponent_points_input, true}
        })
        {
            min_input->setMinimum(record_table_model->getMinimumPoints(is_opponent));
            max_input->setMaximum(record_table_model->getMaximumPoints(is_opponent));

            max_input->setMinimum(min_input->value());
            min_input->setMaximum(max_input->value());
        }

        emit pointsFilterValuesChanged
        (
            minimum_points_input.value(),
            maximum_points_input.value(),
            minimum_opponent_points_input.value(),
            maximum_opponent_points_input.value()
        );

        return;
    }
    
    void RecordDialog::resetFilters()
    {
        maximum_points_input.setValue(int_limits.first);
        minimum_points_input.setValue(int_limits.second);
        maximum_opponent_points_input.setValue(int_limits.first);
        minimum_opponent_points_input.setValue(int_limits.second);

        maximum_points_checkbox.setCheckState(Qt::CheckState::Unchecked);
        minimum_points_checkbox.setCheckState(Qt::CheckState::Unchecked);
        maximum_opponent_points_checkbox.setCheckState(Qt::CheckState::Unchecked);
        minimum_opponent_points_checkbox.setCheckState(Qt::CheckState::Unchecked);

        return;
    }
    
    void RecordDialog::initialisePointsLimits(int& a_layout_row)
    {
        updatePointsLimits();
        updatePointsFilterStatus();

        bool use_upper_limit = false;

        for (auto input : {&maximum_points_input, &minimum_points_input, &maximum_opponent_points_input, &minimum_opponent_points_input})
        {
            connect(input, QOverload<int>::of(&QSpinBox::valueChanged), this, &RecordDialog::updatePointsLimits);
            input->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
            input->setKeyboardTracking(false);
            input->setValue((use_upper_limit = !use_upper_limit) ? int_limits.first : int_limits.second);
        }

        for (auto checkbox : {&maximum_points_checkbox, &minimum_points_checkbox, &maximum_opponent_points_checkbox, &minimum_opponent_points_checkbox})
        {
            connect(checkbox, &QCheckBox::stateChanged, this, &RecordDialog::updatePointsFilterStatus);
        }

        int group_layout_row = 0;

        points_group_layout.addWidget(&maximum_points_checkbox, group_layout_row, 0);
        points_group_layout.addWidget(&maximum_points_label, group_layout_row, 1);
        points_group_layout.addWidget(&maximum_points_input, group_layout_row++, 2);

        points_group_layout.addWidget(&minimum_points_checkbox, group_layout_row, 0);
        points_group_layout.addWidget(&minimum_points_label, group_layout_row, 1);
        points_group_layout.addWidget(&minimum_points_input, group_layout_row++, 2);

        points_group.setLayout(&points_group_layout);
        filter_layout.addWidget(&points_group, a_layout_row++, 0);

        group_layout_row = 0;

        opponent_points_group_layout.addWidget(&maximum_opponent_points_checkbox, group_layout_row, 0);
        opponent_points_group_layout.addWidget(&maximum_opponent_points_label, group_layout_row, 1);
        opponent_points_group_layout.addWidget(&maximum_opponent_points_input, group_layout_row++, 2);

        opponent_points_group_layout.addWidget(&minimum_opponent_points_checkbox, group_layout_row, 0);
        opponent_points_group_layout.addWidget(&minimum_opponent_points_label, group_layout_row, 1);
        opponent_points_group_layout.addWidget(&minimum_opponent_points_input, group_layout_row++, 2);

        opponent_points_group.setLayout(&opponent_points_group_layout);
        filter_layout.addWidget(&opponent_points_group, a_layout_row++, 0);

        return;
    }
}