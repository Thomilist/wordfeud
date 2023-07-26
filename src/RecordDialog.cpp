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
        initialisePointsLimits(filter_layout_row);
        
        int layout_row = 0;

        grid_layout.addLayout(&filter_layout, layout_row++, 0, Qt::AlignCenter);
        grid_layout.addWidget(&reset_filters_button, layout_row++, 0, Qt::AlignCenter);
        grid_layout.addWidget(&filter_padding, layout_row, 0, Qt::AlignCenter);
        grid_layout.setRowStretch(layout_row++, 1);
        grid_layout.addWidget(&buttons, layout_row++, 0, 1, 2, Qt::AlignCenter);

        grid_layout.addWidget(&record_table, 0, 1, layout_row - 1, 1, Qt::AlignCenter);

        record_table.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        filter_padding.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

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
        // Dictionary filter
        fitListToContents(&dictionary_list);

        // Modifier filter
        fitListToContents(&modifier_list);

        // Table
        record_table.setMinimumHeight(750);
        fitTableWidthToContents(&record_table);

        adjustSize();

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

        maximum_points_checkbox.setCheckState(Qt::Unchecked);
        minimum_points_checkbox.setCheckState(Qt::Unchecked);
        maximum_opponent_points_checkbox.setCheckState(Qt::Unchecked);
        minimum_opponent_points_checkbox.setCheckState(Qt::Unchecked);

        for (int index = 0; index < dictionary_list.count(); ++index)
        {
            dictionary_list.item(index)->setCheckState(Qt::Checked);
        }

        for (int index = 0; index < modifier_list.count(); ++index)
        {
            modifier_list.item(index)->setCheckState(Qt::Checked);
        }

        return;
    }
    
    void RecordDialog::repopulateFilters()
    {
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
        a_list->setFixedWidth
        (
            a_list->sizeHint().width()
        );
        a_list->setFixedHeight
        (
            a_list->count() > 0
                ? a_list->sizeHintForRow(0) * a_list->count() + 10
                : 10
        );

        a_list->adjustSize();
        return;
    }
    
    void RecordDialog::fitTableWidthToContents(QTableView* a_table)
    {
        a_table->setMinimumWidth
        (
            a_table->verticalHeader()->width()
            + a_table->horizontalHeader()->length()
            + a_table->verticalScrollBar()->width()
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
        record_table.verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        connect(record_table.horizontalHeader(), &QHeaderView::geometriesChanged, this, &RecordDialog::updateRecordTableWidth);
        
        return;
    }
    
    void RecordDialog::initialiseDictionaryList(int& a_layout_row)
    {
        dictionary_list.setSelectionMode(QAbstractItemView::NoSelection);

        connect(&dictionary_list, &QListWidget::itemChanged, this, &RecordDialog::updateDictionaryFilter);
        connect(this, &RecordDialog::dictionaryFilterChanged, &record_proxy, &RecordSortFilterProxyModel::updateDictionaryFilter);

        populateDictionaryList();

        dictionary_layout.addWidget(&dictionary_list, 0, 0);
        dictionary_group.setLayout(&dictionary_layout);
        filter_layout.addWidget(&dictionary_group, a_layout_row++, 0, Qt::AlignCenter);

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

        connect(&modifier_list, &QListWidget::itemChanged, this, &RecordDialog::updateModifierFilter);
        connect(this, &RecordDialog::modifierFilterChanged, &record_proxy, &RecordSortFilterProxyModel::updateModifierFilter);

        populateModifierList();

        modifier_layout.addWidget(&modifier_list, 0, 0);
        modifier_group.setLayout(&modifier_layout);
        filter_layout.addWidget(&modifier_group, a_layout_row++, 0, Qt::AlignCenter);

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

        points_group.setLayout(&points_group_layout);

        int group_layout_row = 0;

        points_group_layout.addWidget(&maximum_points_checkbox, group_layout_row, 0);
        points_group_layout.addWidget(&maximum_points_label, group_layout_row, 1);
        points_group_layout.addWidget(&maximum_points_padding, group_layout_row, 2);
        points_group_layout.addWidget(&maximum_points_input, group_layout_row++, 3);

        points_group_layout.addWidget(&minimum_points_checkbox, group_layout_row, 0);
        points_group_layout.addWidget(&minimum_points_label, group_layout_row, 1);
        points_group_layout.addWidget(&minimum_points_padding, group_layout_row, 2);
        points_group_layout.addWidget(&minimum_points_input, group_layout_row++, 3);

        points_group.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        maximum_points_padding.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        minimum_points_padding.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        points_group_layout.setColumnStretch(2, 1);

        filter_layout.addWidget(&points_group, a_layout_row++, 0, Qt::AlignCenter);

        opponent_points_group.setLayout(&opponent_points_group_layout);

        group_layout_row = 0;

        opponent_points_group_layout.addWidget(&maximum_opponent_points_checkbox, group_layout_row, 0);
        opponent_points_group_layout.addWidget(&maximum_opponent_points_label, group_layout_row, 1);
        opponent_points_group_layout.addWidget(&maximum_opponent_points_padding, group_layout_row, 2);
        opponent_points_group_layout.addWidget(&maximum_opponent_points_input, group_layout_row++, 3);

        opponent_points_group_layout.addWidget(&minimum_opponent_points_checkbox, group_layout_row, 0);
        opponent_points_group_layout.addWidget(&minimum_opponent_points_label, group_layout_row, 1);
        opponent_points_group_layout.addWidget(&minimum_opponent_points_padding, group_layout_row, 2);
        opponent_points_group_layout.addWidget(&minimum_opponent_points_input, group_layout_row++, 3);

        opponent_points_group.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        maximum_opponent_points_padding.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        minimum_opponent_points_padding.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        opponent_points_group_layout.setColumnStretch(2, 1);

        filter_layout.addWidget(&opponent_points_group, a_layout_row++, 0, Qt::AlignCenter);

        return;
    }
}