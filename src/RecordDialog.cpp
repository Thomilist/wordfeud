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
        connect(&record_proxy, &RecordSortFilterProxyModel::rowsInserted, this, &RecordDialog::updateDisplayCountLabel);
        connect(&record_proxy, &RecordSortFilterProxyModel::rowsRemoved, this, &RecordDialog::updateDisplayCountLabel);

        setRecordSource(a_record_table_model);
        initialiseRecordTable();

        int filter_layout_row = 0;

        QFont display_count_font;
        display_count_font.setPixelSize(24);
        display_count_label.setFont(display_count_font);
        filter_layout.addWidget(&display_count_label, filter_layout_row++, 0, 1, 2);

        initialiseDictionaryList(filter_layout_row);
        initialiseModifierList(filter_layout_row);
        initialiseNameField(filter_layout_row);
        initialiseOpponentNameField(filter_layout_row);
        initialiseControlList(filter_layout_row);
        initialiseOpponentControlList(filter_layout_row);
        initialisePointsLimits(filter_layout_row);
        initialiseDateTimeLimits(filter_layout_row);
        
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
        connect(record_table_model, &RecordTableModel::filterDataUpdated, this, &RecordDialog::prepareView);
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
    
    void RecordDialog::updateFilterItems()
    {
        repopulateFilters();
        updatePointsLimits();
        updateDateTimeLimits();
        return;
    }
    
    void RecordDialog::updateControlFilter()
    {
        updateFilterList(control_list, checked_control_items);
        emit controlFilterChanged(checked_control_items);
        return;
    }
    
    void RecordDialog::updateOpponentControlFilter()
    {
        updateFilterList(opponent_control_list, checked_opponent_control_items);
        emit opponentControlFilterChanged(checked_opponent_control_items);
        return;
    }
    
    void RecordDialog::updateDictionaryFilter()
    {
        updateFilterList(dictionary_list, checked_dictionary_items);
        emit dictionaryFilterChanged(checked_dictionary_items);
        return;
    }
    
    void RecordDialog::updateModifierFilter()
    {
        updateFilterList(modifier_list, checked_modifier_items);
        emit modifierFilterChanged(checked_modifier_items);
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
    
    void RecordDialog::updateDateTimeFilterStatus()
    {
        for (auto [checkbox, input] :{
            std::pair{&datetime_after_checkbox, &datetime_after_input},
            std::pair{&datetime_before_checkbox, &datetime_before_input},
        })
        {
            input->setEnabled(checkbox->isChecked());
        }
        
        emit dateTimeFilterStatusChanged(
            datetime_after_checkbox.isChecked(),
            datetime_before_checkbox.isChecked());
        
        return;
    }
    
    void RecordDialog::updateDateTimeLimits()
    {
        emit dateTimeFilterValuesChanged(
            datetime_after_input.dateTime(),
            datetime_before_input.dateTime()
        );

        return;
    }
    
    void RecordDialog::resetFilters()
    {
        // Control
        for (int index = 0; index < control_list.count(); ++index)
        {
            control_list.item(index)->setCheckState(Qt::Checked);
        }

        // Opponent control
        for (int index = 0; index < opponent_control_list.count(); ++index)
        {
            opponent_control_list.item(index)->setCheckState(Qt::Checked);
        }

        // Dictionary
        for (int index = 0; index < dictionary_list.count(); ++index)
        {
            dictionary_list.item(index)->setCheckState(Qt::Checked);
        }

        // Modifier
        for (int index = 0; index < modifier_list.count(); ++index)
        {
            modifier_list.item(index)->setCheckState(Qt::Checked);
        }

        // Name, opponent name
        name_input.clear();
        opponent_name_input.clear();

        // Points
        maximum_points_checkbox.setCheckState(Qt::Unchecked);
        minimum_points_checkbox.setCheckState(Qt::Unchecked);
        maximum_points_input.setValue(record_table_model->getMaximumPoints());
        minimum_points_input.setValue(record_table_model->getMinimumPoints());

        // Opponent points
        maximum_opponent_points_checkbox.setCheckState(Qt::Unchecked);
        minimum_opponent_points_checkbox.setCheckState(Qt::Unchecked);
        maximum_opponent_points_input.setValue(record_table_model->getMaximumPoints(true));
        minimum_opponent_points_input.setValue(record_table_model->getMinimumPoints(true));

        // Date/time
        datetime_after_checkbox.setCheckState(Qt::Unchecked);
        datetime_before_checkbox.setCheckState(Qt::Unchecked);
        resetEarliestDateTime();
        resetLatestDateTime();

        return;
    }
    
    void RecordDialog::resetEarliestDateTime()
    {
        datetime_after_input.setDateTime(record_table_model->getEarliestDate());
        return;
    }
    
    void RecordDialog::resetLatestDateTime()
    {
        datetime_before_input.setDateTime(record_table_model->getLatestDate());
        return;
    }
    
    void RecordDialog::repopulateFilters()
    {
        populateFilterList(
            control_list,
            record_table_model->getControlEntries(),
            control_items,
            checked_control_items);
        populateFilterList(
            opponent_control_list,
            record_table_model->getOpponentControlEntries(),
            opponent_control_items,
            checked_opponent_control_items);
        populateFilterList(
            dictionary_list,
            record_table_model->getDictionaries(),
            dictionary_items,
            checked_dictionary_items);
        populateFilterList(
            modifier_list,
            record_table_model->getModifiers(),
            modifier_items,
            checked_modifier_items);
        return;
    }
    
    void RecordDialog::prepareView()
    {
        updateFilterItems();
        updateMinimumSize();
        return;
    }
    
    void RecordDialog::updateRecordTableWidth()
    {
        fitTableWidthToContents(&record_table);
        return;
    }
    
    void RecordDialog::updateDisplayCountLabel()
    {
        display_count_label.setText
        (
            QString()
            % "Showing "
            % QString::number(record_proxy.rowCount())
            % " / "
            % QString::number(record_proxy.sourceModel()->rowCount())
            % " entries"
        );

        return;
    }
    
    template<typename Container>
    void RecordDialog::populateFilterList(QListWidget& a_list,
                    const Container& a_source,
                    Container& a_existing_items,
                    Container& a_checked_items)
    {
        a_list.clear();
        bool checked = false;

        for (auto entry : a_source)
        {
            QListWidgetItem* item = new QListWidgetItem(entry, &a_list);
            checked = a_checked_items.contains(entry) || !a_existing_items.contains(entry);
            item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
            a_list.addItem(item);
            a_existing_items.insert(entry);

            if (checked)
            {
                a_checked_items.insert(entry);
            }
        }

        return;
    }
    
    template<typename Container>
    void RecordDialog::updateFilterList(QListWidget& a_list, Container& a_checked_items)
    {
        QListWidgetItem* item;

        for (int index = 0; index < a_list.count(); ++index)
        {
            item = a_list.item(index);
            auto checkstate = item->checkState();

            if (checkstate == Qt::Checked)
            {
                a_checked_items.insert(item->text());
            }
            else if (checkstate == Qt::Unchecked)
            {
                a_checked_items.erase(item->text());
            }
        }

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
        record_table.hideColumn(RecordColumn::DateTime);

        connect(record_table.horizontalHeader(), &QHeaderView::geometriesChanged, this, &RecordDialog::updateRecordTableWidth);
        
        return;
    }
    
    void RecordDialog::initialiseControlList(int& a_layout_row)
    {
        control_list.setSelectionMode(QAbstractItemView::NoSelection);
        control_list.setMaximumWidth(half_filter_width);

        connect(&control_list, &QListWidget::itemChanged, this, &RecordDialog::updateControlFilter);
        connect(this, &RecordDialog::controlFilterChanged, &record_proxy, &RecordSortFilterProxyModel::updateControlFilter);

        checked_control_items = record_table_model->getControlEntries();
        populateFilterList(
            control_list,
            record_table_model->getControlEntries(),
            control_items,
            checked_control_items);

        control_layout.addWidget(&control_list, 0, 0);
        control_group.setLayout(&control_layout);
        filter_layout.addWidget(&control_group, a_layout_row, 0);

        return;
    }
    
    void RecordDialog::initialiseOpponentControlList(int& a_layout_row)
    {
        opponent_control_list.setSelectionMode(QAbstractItemView::NoSelection);
        opponent_control_list.setMaximumWidth(half_filter_width);

        connect(&opponent_control_list, &QListWidget::itemChanged, this, &RecordDialog::updateOpponentControlFilter);
        connect(this, &RecordDialog::opponentControlFilterChanged, &record_proxy, &RecordSortFilterProxyModel::updateOpponentControlFilter);

        checked_opponent_control_items = record_table_model->getOpponentControlEntries();
        populateFilterList(
            opponent_control_list,
            record_table_model->getOpponentControlEntries(),
            opponent_control_items,
            checked_opponent_control_items);

        opponent_control_layout.addWidget(&opponent_control_list, 0, 0);
        opponent_control_group.setLayout(&opponent_control_layout);
        filter_layout.addWidget(&opponent_control_group, a_layout_row++, 1);

        return;
    }
    
    void RecordDialog::initialiseDictionaryList(int& a_layout_row)
    {
        dictionary_list.setSelectionMode(QAbstractItemView::NoSelection);
        dictionary_list.setFixedWidth(half_filter_width);

        connect(&dictionary_list, &QListWidget::itemChanged, this, &RecordDialog::updateDictionaryFilter);
        connect(this, &RecordDialog::dictionaryFilterChanged, &record_proxy, &RecordSortFilterProxyModel::updateDictionaryFilter);

        checked_dictionary_items = record_table_model->getDictionaries();
        populateFilterList(
            dictionary_list,
            record_table_model->getDictionaries(),
            dictionary_items,
            checked_dictionary_items);

        dictionary_layout.addWidget(&dictionary_list, 0, 0);
        dictionary_group.setLayout(&dictionary_layout);
        filter_layout.addWidget(&dictionary_group, a_layout_row, 0);

        return;
    }
    
    void RecordDialog::initialiseModifierList(int& a_layout_row)
    {
        modifier_list.setSelectionMode(QAbstractItemView::NoSelection);
        modifier_list.setFixedWidth(half_filter_width);

        connect(&modifier_list, &QListWidget::itemChanged, this, &RecordDialog::updateModifierFilter);
        connect(this, &RecordDialog::modifierFilterChanged, &record_proxy, &RecordSortFilterProxyModel::updateModifierFilter);

        checked_modifier_items = record_table_model->getModifiers();
        populateFilterList(
            modifier_list,
            record_table_model->getModifiers(),
            modifier_items,
            checked_modifier_items);

        modifier_layout.addWidget(&modifier_list, 0, 0);
        modifier_group.setLayout(&modifier_layout);
        filter_layout.addWidget(&modifier_group, a_layout_row++, 1);

        return;
    }
    
    void RecordDialog::initialiseNameField(int& a_layout_row)
    {
        name_input.setFixedWidth(half_filter_width);

        connect(&name_input, &QLineEdit::textChanged, &record_proxy, &RecordSortFilterProxyModel::updateNameFilter);

        name_layout.addWidget(&name_input, 0, 0);
        name_group.setLayout(&name_layout);
        filter_layout.addWidget(&name_group, a_layout_row, 0);

        return;
    }
    
    void RecordDialog::initialiseOpponentNameField(int& a_layout_row)
    {
        opponent_name_input.setFixedWidth(half_filter_width);

        connect(&opponent_name_input, &QLineEdit::textChanged, &record_proxy, &RecordSortFilterProxyModel::updateOpponentNameFilter);

        opponent_name_layout.addWidget(&opponent_name_input, 0, 0);
        opponent_name_group.setLayout(&opponent_name_layout);
        filter_layout.addWidget(&opponent_name_group, a_layout_row++, 1);
        
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

            // use_upper_limit flips on every iteration
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
    
    void RecordDialog::initialiseDateTimeLimits(int& a_layout_row)
    {
        connect(this, &RecordDialog::dateTimeFilterValuesChanged, &record_proxy, &RecordSortFilterProxyModel::updateDateTimeFilterValues);
        connect(this, &RecordDialog::dateTimeFilterStatusChanged, &record_proxy, &RecordSortFilterProxyModel::enableDateTimeFilters);

        connect(&datetime_after_reset_button, &QPushButton::clicked, this, &RecordDialog::resetEarliestDateTime);
        connect(&datetime_before_reset_button, &QPushButton::clicked, this, &RecordDialog::resetLatestDateTime);

        datetime_after_input.setDisplayFormat(RecordTableModel::getDateTimeFormat());
        datetime_before_input.setDisplayFormat(RecordTableModel::getDateTimeFormat());

        datetime_after_input.setDateTime(record_table_model->getEarliestDate());
        datetime_before_input.setDateTime(record_table_model->getLatestDate());

        datetime_after_input.setCalendarPopup(true);
        datetime_before_input.setCalendarPopup(true);

        datetime_after_input.setFixedWidth(full_filter_width);
        datetime_before_input.setFixedWidth(full_filter_width);

        updateDateTimeLimits();
        updateDateTimeFilterStatus();

        for (auto input : {&datetime_after_input, &datetime_before_input})
        {
            connect(input, &QDateTimeEdit::dateTimeChanged, this, &RecordDialog::updateDateTimeLimits);
            input->setKeyboardTracking(false);
        }

        for (auto checkbox : {&datetime_after_checkbox, &datetime_before_checkbox})
        {
            connect(checkbox, &QCheckBox::stateChanged, this, &RecordDialog::updateDateTimeFilterStatus);
        }

        datetime_layout.addWidget(&datetime_after_checkbox, 0, 0);
        datetime_layout.addWidget(&datetime_after_label, 0, 1);
        datetime_layout.addWidget(&datetime_after_input, 1, 0, 1, 2);
        datetime_layout.addWidget(&datetime_after_reset_button, 2, 0, 1, 2);

        datetime_layout.addWidget(&datetime_before_checkbox, 0, 2);
        datetime_layout.addWidget(&datetime_before_label, 0, 3);
        datetime_layout.addWidget(&datetime_before_input, 1, 2, 1, 2);
        datetime_layout.addWidget(&datetime_before_reset_button, 2, 2, 1, 2);

        datetime_layout.setColumnStretch(0, 0);
        datetime_layout.setColumnStretch(1, 1);
        datetime_layout.setColumnStretch(2, 0);
        datetime_layout.setColumnStretch(3, 1);

        datetime_group.setLayout(&datetime_layout);
        filter_layout.addWidget(&datetime_group, a_layout_row++, 0, 1, 2);

        return;
    }
}