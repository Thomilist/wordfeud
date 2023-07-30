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
        connect(&reset_filters_button, &QPushButton::clicked, this, &RecordDialog::resetFilters);

        setRecordSource(a_record_table_model);
        initialiseRecordTable();

        int filter_layout_row = 0;

        initialiseDictionaryList(filter_layout_row);
        initialiseModifierList(filter_layout_row);
        initialiseControlList(filter_layout_row);
        initialiseOpponentControlList(filter_layout_row);
        initialisePointsLimits(filter_layout_row);
        
        int layout_row = 0;

        grid_layout.addLayout(&filter_layout, layout_row++, 0, Qt::AlignCenter);
        grid_layout.addWidget(&reset_filters_button, layout_row++, 0);
        grid_layout.addWidget(&filter_padding, layout_row, 0, Qt::AlignCenter);
        grid_layout.setRowStretch(layout_row++, 1);
        grid_layout.addWidget(&buttons, layout_row++, 0, 1, 2, Qt::AlignCenter);

        grid_layout.addWidget(&record_table, 0, 1, layout_row - 1, 1, Qt::AlignCenter);

        record_table.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        filter_padding.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        reset_filters_button.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

        reset_filters_button.setAutoDefault(false);

        setLayout(&grid_layout);
        layout()->setSizeConstraint(QLayout::SetFixedSize);
    }
    
    RecordDialog::~RecordDialog()
    { }
    
    void RecordDialog::open()
    {
        prepareView();
        QDialog::open();
        return;
    }
    
    void RecordDialog::setRecordSource(RecordTableModel* a_record_table_model)
    {
        record_table_model = a_record_table_model;
        record_proxy.setSourceModel(record_table_model);
        connect(record_table_model, &QAbstractItemModel::rowsInserted, this, &RecordDialog::prepareView);
        connect(record_table_model, &RecordTableModel::pointsLimitsChanged, this, &RecordDialog::updatePointsLimits);
        return;
    }
    
    void RecordDialog::updateMinimumSize()
    {
        // Control filters
        fitListToContents(&control_list, &opponent_control_list);
        
        // Dictionary and modifier filters
        fitListToContents(&dictionary_list, &modifier_list);

        // Table
        record_table.setFixedHeight(table_height);
        fitTableWidthToContents(&record_table);

        adjustSize();

        return;
    }
    
    void RecordDialog::updateControlFilter()
    {
        std::set<QString, ScoreControlCompare> control_filter_set;
        QListWidgetItem* control_item;

        for (int index = 0; index < control_list.count(); ++index)
        {
            control_item = control_list.item(index);

            if (control_item->checkState() == Qt::Checked)
            {
                control_filter_set.insert(control_item->text());
            }
        }

        emit controlFilterChanged(control_filter_set);
        return;
    }
    
    void RecordDialog::updateOpponentControlFilter()
    {
        std::set<QString, ScoreControlCompare> opponent_control_filter_set;
        QListWidgetItem* opponent_control_item;

        for (int index = 0; index < opponent_control_list.count(); ++index)
        {
            opponent_control_item = opponent_control_list.item(index);

            if (opponent_control_item->checkState() == Qt::Checked)
            {
                opponent_control_filter_set.insert(opponent_control_item->text());
            }
        }

        emit opponentControlFilterChanged(opponent_control_filter_set);
        return;
    }
    
    void RecordDialog::updateDictionaryFilter()
    {
        std::set<QString> dictionary_filter_set;
        QListWidgetItem* dictionary_item;

        for (int index = 0; index < dictionary_list.count(); ++index)
        {
            dictionary_item = dictionary_list.item(index);

            if (dictionary_item->checkState() == Qt::Checked)
            {
                dictionary_filter_set.insert(dictionary_item->text());
            }
        }

        emit dictionaryFilterChanged(dictionary_filter_set);
        return;
    }
    
    void RecordDialog::updateModifierFilter()
    {
        std::set<QString> modifier_filter_set;
        QListWidgetItem* modifier_item;

        for (int index = 0; index < modifier_list.count(); ++index)
        {
            modifier_item = modifier_list.item(index);

            if (modifier_item->checkState() == Qt::Checked)
            {
                modifier_filter_set.insert(modifier_item->text());
            }
        }

        emit modifierFilterChanged(modifier_filter_set);
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
        int min_value;
        int max_value;
        
        for (auto [min_input, max_input, is_opponent] :
        {
            std::tuple{&minimum_points_input, &maximum_points_input, false},
            std::tuple{&minimum_opponent_points_input, &maximum_opponent_points_input, true}
        })
        {
            min_value = record_table_model->getMinimumPoints(is_opponent);
            max_value = record_table_model->getMaximumPoints(is_opponent);
            
            if (min_input->value() < min_value)
            {
                min_input->setValue(min_value);
            }
            else if (min_input->value() > max_value)
            {
                min_input->setValue(max_value);
            }

            if (max_input->value() > max_value)
            {
                max_input->setValue(max_value);
            }
            else if (max_input->value() < min_value)
            {
                max_input->setValue(min_value);
            }
        }

        if (minimum_points_input.value() != last_points_filter_values[0])
        {
            if (maximum_points_input.value() < minimum_points_input.value())
            {
                maximum_points_input.setValue(minimum_points_input.value());
            }
        }
        else if (maximum_points_input.value() != last_points_filter_values[1])
        {
            if (minimum_points_input.value() > maximum_points_input.value())
            {
                minimum_points_input.setValue(maximum_points_input.value());
            }
        }

        if (minimum_opponent_points_input.value() != last_points_filter_values[0])
        {
            if (maximum_opponent_points_input.value() < minimum_opponent_points_input.value())
            {
                maximum_opponent_points_input.setValue(minimum_opponent_points_input.value());
            }
        }
        else if (maximum_opponent_points_input.value() != last_points_filter_values[1])
        {
            if (minimum_opponent_points_input.value() > maximum_opponent_points_input.value())
            {
                minimum_opponent_points_input.setValue(maximum_opponent_points_input.value());
            }
        }

        updateLastPointsLimits();

        emit pointsFilterValuesChanged
        (
            minimum_points_input.value(),
            maximum_points_input.value(),
            minimum_opponent_points_input.value(),
            maximum_opponent_points_input.value()
        );

        return;
    }
    
    void RecordDialog::updateLastPointsLimits()
    {
        last_points_filter_values =
        {
            minimum_points_input.value(),
            maximum_points_input.value(),
            minimum_opponent_points_input.value(),
            maximum_opponent_points_input.value()
        };

        return;
    }
    
    void RecordDialog::resetFilters()
    {
        for (int index = 0; index < control_list.count(); ++index)
        {
            control_list.item(index)->setCheckState(Qt::Checked);
        }

        for (int index = 0; index < opponent_control_list.count(); ++index)
        {
            opponent_control_list.item(index)->setCheckState(Qt::Checked);
        }

        for (int index = 0; index < dictionary_list.count(); ++index)
        {
            dictionary_list.item(index)->setCheckState(Qt::Checked);
        }

        for (int index = 0; index < modifier_list.count(); ++index)
        {
            modifier_list.item(index)->setCheckState(Qt::Checked);
        }

        maximum_points_input.setValue(record_table_model->getMaximumPoints());
        minimum_points_input.setValue(record_table_model->getMinimumPoints());
        maximum_opponent_points_input.setValue(record_table_model->getMaximumPoints(true));
        minimum_opponent_points_input.setValue(record_table_model->getMinimumPoints(true));

        maximum_points_checkbox.setCheckState(Qt::Unchecked);
        minimum_points_checkbox.setCheckState(Qt::Unchecked);
        maximum_opponent_points_checkbox.setCheckState(Qt::Unchecked);
        minimum_opponent_points_checkbox.setCheckState(Qt::Unchecked);

        return;
    }
    
    void RecordDialog::repopulateFilters()
    {
        populateControlList();
        populateOpponentControlList();
        populateDictionaryList();
        populateModifierList();
        return;
    }
    
    void RecordDialog::prepareView()
    {
        repopulateFilters();
        updateMinimumSize();
        return;
    }
    
    void RecordDialog::updateRecordTableWidth()
    {
        fitTableWidthToContents(&record_table);
        return;
    }
    
    void RecordDialog::fitListToContents(QListWidget* a_list)
    {
        int height = 10;
        int max_height = a_list->sizeHintForRow(0) * 5 + 10;

        if (a_list->count() > 0)
        {
            height = a_list->sizeHintForRow(0) * a_list->count() + 10;
        }

        if (height > max_height)
        {
            height = max_height;
        }

        a_list->setFixedHeight(height);
        a_list->adjustSize();
        return;
    }
    
    void RecordDialog::fitListToContents(QListWidget* a_first_list, QListWidget* a_second_list)
    {
        int first_height = 10;
        int second_height = 10;
        int max_height = a_first_list->sizeHintForRow(0) * 5 + 10;

        if (a_first_list->count() > 0)
        {
            first_height = a_first_list->sizeHintForRow(0) * a_first_list->count() + 10;
        }

        if (a_second_list->count() > 0)
        {
            second_height = a_second_list->sizeHintForRow(0) * a_second_list->count() + 10;
        }

        int height = first_height > second_height ? first_height : second_height;

        if (height > max_height)
        {
            height = max_height;
        }

        a_first_list->setFixedHeight(height);
        a_second_list->setFixedHeight(height);
        a_first_list->adjustSize();
        a_second_list->adjustSize();
    }
    
    void RecordDialog::fitTableWidthToContents(QTableView* a_table)
    {
        a_table->resizeColumnsToContents();
        a_table->setMinimumWidth
        (
            a_table->verticalHeader()->width()
            + a_table->horizontalHeader()->length()
            // Goodbye, horisontal scroll bar. You won't be missed >:(
            + ((a_table->verticalHeader()->length() + a_table->horizontalHeader()->height()) > table_height ? a_table->verticalScrollBar()->width() : 0)
        );
        a_table->adjustSize();
        return;
    }
    
    void RecordDialog::initialiseRecordTable()
    {
        record_table.setModel(&record_proxy);
        record_table.setSelectionMode(QAbstractItemView::NoSelection);
        record_table.setEditTriggers(QAbstractItemView::NoEditTriggers);
        record_table.horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        record_table.verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

        connect(record_table.horizontalHeader(), &QHeaderView::geometriesChanged, this, &RecordDialog::updateRecordTableWidth);
        
        return;
    }
    
    void RecordDialog::initialiseControlList(int& a_layout_row)
    {
        control_list.setSelectionMode(QAbstractItemView::NoSelection);
        control_list.setMaximumWidth(half_filter_width);

        connect(&control_list, &QListWidget::itemChanged, this, &RecordDialog::updateControlFilter);
        connect(this, &RecordDialog::controlFilterChanged, &record_proxy, &RecordSortFilterProxyModel::updateControlFilter);

        populateControlList();

        control_layout.addWidget(&control_list, 0, 0);
        control_group.setLayout(&control_layout);
        filter_layout.addWidget(&control_group, a_layout_row, 0);

        return;
    }
    
    void RecordDialog::populateControlList()
    {
        control_list.clear();

        for (auto control : record_table_model->getControlEntries())
        {
            QListWidgetItem* item = new QListWidgetItem(control, &control_list);
            item->setCheckState(Qt::Checked);
            control_list.addItem(item);
        }

        return;
    }
    
    void RecordDialog::initialiseOpponentControlList(int& a_layout_row)
    {
        opponent_control_list.setSelectionMode(QAbstractItemView::NoSelection);
        opponent_control_list.setMaximumWidth(half_filter_width);

        connect(&opponent_control_list, &QListWidget::itemChanged, this, &RecordDialog::updateOpponentControlFilter);
        connect(this, &RecordDialog::opponentControlFilterChanged, &record_proxy, &RecordSortFilterProxyModel::updateOpponentControlFilter);

        populateOpponentControlList();

        opponent_control_layout.addWidget(&opponent_control_list, 0, 0);
        opponent_control_group.setLayout(&opponent_control_layout);
        filter_layout.addWidget(&opponent_control_group, a_layout_row++, 1);

        return;
    }
    
    void RecordDialog::populateOpponentControlList()
    {
        opponent_control_list.clear();

        for (auto opponent_control : record_table_model->getOpponentControlEntries())
        {
            QListWidgetItem* item = new QListWidgetItem(opponent_control, &opponent_control_list);
            item->setCheckState(Qt::Checked);
            opponent_control_list.addItem(item);
        }

        return;
    }
    
    void RecordDialog::initialiseDictionaryList(int& a_layout_row)
    {
        dictionary_list.setSelectionMode(QAbstractItemView::NoSelection);
        dictionary_list.setFixedWidth(half_filter_width);

        connect(&dictionary_list, &QListWidget::itemChanged, this, &RecordDialog::updateDictionaryFilter);
        connect(this, &RecordDialog::dictionaryFilterChanged, &record_proxy, &RecordSortFilterProxyModel::updateDictionaryFilter);

        populateDictionaryList();

        dictionary_layout.addWidget(&dictionary_list, 0, 0);
        dictionary_group.setLayout(&dictionary_layout);
        filter_layout.addWidget(&dictionary_group, a_layout_row, 0);

        return;
    }
    
    void RecordDialog::populateDictionaryList()
    {
        dictionary_list.clear();

        for (auto dictionary : record_table_model->getDictionaries())
        {
            QListWidgetItem* item = new QListWidgetItem(dictionary, &dictionary_list);
            item->setCheckState(Qt::Checked);
            dictionary_list.addItem(item);
        }

        return;
    }
    
    void RecordDialog::initialiseModifierList(int& a_layout_row)
    {
        modifier_list.setSelectionMode(QAbstractItemView::NoSelection);
        modifier_list.setFixedWidth(half_filter_width);

        connect(&modifier_list, &QListWidget::itemChanged, this, &RecordDialog::updateModifierFilter);
        connect(this, &RecordDialog::modifierFilterChanged, &record_proxy, &RecordSortFilterProxyModel::updateModifierFilter);

        populateModifierList();

        modifier_layout.addWidget(&modifier_list, 0, 0);
        modifier_group.setLayout(&modifier_layout);
        filter_layout.addWidget(&modifier_group, a_layout_row++, 1);

        return;
    }
    
    void RecordDialog::populateModifierList()
    {
        modifier_list.clear();

        for (auto modifier : record_table_model->getModifiers())
        {
            QListWidgetItem* item = new QListWidgetItem(modifier, &modifier_list);
            item->setCheckState(Qt::Checked);
            modifier_list.addItem(item);
        }

        return;
    }
    
    void RecordDialog::initialisePointsLimits(int& a_layout_row)
    {
        connect(this, &RecordDialog::pointsFilterValuesChanged, &record_proxy, &RecordSortFilterProxyModel::updatePointsFilterValues);
        connect(this, &RecordDialog::pointsFilterStatusChanged, &record_proxy, &RecordSortFilterProxyModel::enablePointsFilters);

        minimum_points_input.setMaximum(int_limits.first);
        minimum_points_input.setMinimum(int_limits.second);
        maximum_points_input.setMaximum(int_limits.first);
        maximum_points_input.setMinimum(int_limits.second);

        minimum_opponent_points_input.setMaximum(int_limits.first);
        minimum_opponent_points_input.setMinimum(int_limits.second);
        maximum_opponent_points_input.setMaximum(int_limits.first);
        maximum_opponent_points_input.setMinimum(int_limits.second);

        minimum_points_input.setValue(int_limits.second);
        maximum_points_input.setValue(int_limits.first);

        minimum_opponent_points_input.setValue(int_limits.second);
        maximum_opponent_points_input.setValue(int_limits.first);

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

        // Points layout
        int group_layout_row = 0;
        points_group.setLayout(&points_group_layout);

        points_group_layout.addWidget(&maximum_points_checkbox, group_layout_row, 0);
        points_group_layout.addWidget(&maximum_points_label, group_layout_row, 1);
        points_group_layout.addWidget(&maximum_points_input, group_layout_row++, 2);

        points_group_layout.addWidget(&minimum_points_checkbox, group_layout_row, 0);
        points_group_layout.addWidget(&minimum_points_label, group_layout_row, 1);
        points_group_layout.addWidget(&minimum_points_input, group_layout_row++, 2);

        maximum_points_input.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        minimum_points_input.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        maximum_points_input.setAlignment(Qt::AlignRight);
        minimum_points_input.setAlignment(Qt::AlignRight);

        // Opponent points layout
        group_layout_row = 0;
        opponent_points_group.setLayout(&opponent_points_group_layout);

        opponent_points_group_layout.addWidget(&maximum_opponent_points_checkbox, group_layout_row, 0);
        opponent_points_group_layout.addWidget(&maximum_opponent_points_label, group_layout_row, 1);
        opponent_points_group_layout.addWidget(&maximum_opponent_points_input, group_layout_row++, 2);

        opponent_points_group_layout.addWidget(&minimum_opponent_points_checkbox, group_layout_row, 0);
        opponent_points_group_layout.addWidget(&minimum_opponent_points_label, group_layout_row, 1);
        opponent_points_group_layout.addWidget(&minimum_opponent_points_input, group_layout_row++, 2);

        maximum_opponent_points_input.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        minimum_opponent_points_input.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        maximum_opponent_points_input.setAlignment(Qt::AlignRight);
        minimum_opponent_points_input.setAlignment(Qt::AlignRight);

        // Add to main filter layout
        filter_layout.addWidget(&points_group, a_layout_row, 0);
        filter_layout.addWidget(&opponent_points_group, a_layout_row++, 1);

        return;
    }
}