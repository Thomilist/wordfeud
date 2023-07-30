#include "RecordSortFilterProxyModel.hpp"


namespace wf
{
    RecordSortFilterProxyModel::RecordSortFilterProxyModel(QObject* a_parent)
        : QSortFilterProxyModel(a_parent)
    {
        name_pattern.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        opponent_name_pattern.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    }
    
    RecordSortFilterProxyModel::~RecordSortFilterProxyModel()
    { }
    
    QVariant RecordSortFilterProxyModel::headerData(int a_section, Qt::Orientation a_orientation, int a_role) const 
    {
        switch (a_orientation)
        {
            case Qt::Orientation::Horizontal:
            {
                return sourceModel()->headerData(a_section, a_orientation, a_role);
            }
            case Qt::Orientation::Vertical:
            {
                switch (a_role)
                {
                    case Qt::DisplayRole:
                    {
                        return a_section + 1;
                    }
                    case Qt::TextAlignmentRole:
                    {
                        return Qt::AlignCenter;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }

        return QVariant();
    }
    
    void RecordSortFilterProxyModel::updateControlFilter(std::set<QString, ScoreControlCompare> a_controls)
    {
        controls = a_controls;
        invalidateFilter();
        return;
    }
    
    void RecordSortFilterProxyModel::updateOpponentControlFilter(std::set<QString, ScoreControlCompare> a_opponent_controls)
    {
        opponent_controls = a_opponent_controls;
        invalidateFilter();
        return;
    }
    
    void RecordSortFilterProxyModel::updateNameFilter(const QString& a_name)
    {
        name_pattern.setPattern(a_name);
        invalidateFilter();
        return;
    }
    
    void RecordSortFilterProxyModel::updateOpponentNameFilter(const QString& a_opponent_name)
    {
        opponent_name_pattern.setPattern(a_opponent_name);
        invalidateFilter();
        return;
    }
    
    void RecordSortFilterProxyModel::updateDictionaryFilter(std::set<QString> a_dictionaries)
    {
        dictionaries = a_dictionaries;
        invalidateFilter();
        return;
    }
    
    void RecordSortFilterProxyModel::updateModifierFilter(std::set<QString> a_modifiers)
    {
        modifiers = a_modifiers;
        invalidateFilter();
        return;
    }
    
    void RecordSortFilterProxyModel::enablePointsFilters(
        bool a_minimum_points_enabled,
        bool a_maximum_points_enabled,
        bool a_minimum_opponent_points_enabled,
        bool a_maximum_opponent_points_enabled)
    {
        minimum_points_enabled = a_minimum_points_enabled;
        maximum_points_enabled = a_maximum_points_enabled;
        minimum_opponent_points_enabled = a_minimum_opponent_points_enabled;
        maximum_opponent_points_enabled = a_maximum_opponent_points_enabled;

        invalidateFilter();

        return;
    }
    
    void RecordSortFilterProxyModel::updatePointsFilterValues(
        int a_minimum_points,
        int a_maximum_points,
        int a_minimum_opponent_points,
        int a_maximum_opponent_points)
    {
        minimum_points = a_minimum_points;
        maximum_points = a_maximum_points;
        minimum_opponent_points = a_minimum_opponent_points;
        maximum_opponent_points = a_maximum_opponent_points;

        invalidateFilter();

        return;
    }
    
    bool RecordSortFilterProxyModel::filterAcceptsRow(int a_source_row, const QModelIndex& a_source_parent) const 
    {
        if (!controlValid(a_source_row, a_source_parent)) {return false;}
        if (!dictionaryValid(a_source_row, a_source_parent)) {return false;}
        if (!modifierValid(a_source_row, a_source_parent)) {return false;}
        if (!nameValid(a_source_row, a_source_parent)) {return false;}
        if (!pointsInRange(a_source_row, a_source_parent)) {return false;}

        return true;
    }
    
    bool RecordSortFilterProxyModel::controlValid(int a_source_row, const QModelIndex& a_source_parent) const
    {
        QModelIndex control_index = sourceModel()->index(a_source_row, RecordColumn::Control, a_source_parent);
        QModelIndex opponent_control_index = sourceModel()->index(a_source_row, RecordColumn::OpponentControl, a_source_parent);
        QString control = sourceModel()->data(control_index).toString();
        QString opponent_control = sourceModel()->data(opponent_control_index).toString();
        return controls.contains(control) && opponent_controls.contains(opponent_control);
    }
    
    bool RecordSortFilterProxyModel::dictionaryValid(int a_source_row, const QModelIndex& a_source_parent) const
    {
        QModelIndex dictionary_index = sourceModel()->index(a_source_row, RecordColumn::Dictionary, a_source_parent);
        QString dictionary = sourceModel()->data(dictionary_index).toString();
        return dictionaries.contains(dictionary);
    }
    
    bool RecordSortFilterProxyModel::modifierValid(int a_source_row, const QModelIndex& a_source_parent) const
    {
        QModelIndex modifier_index = sourceModel()->index(a_source_row, RecordColumn::Modifiers, a_source_parent);
        QString modifier = sourceModel()->data(modifier_index).toString();
        return modifiers.contains(modifier);
    }
    
    bool RecordSortFilterProxyModel::nameValid(int a_source_row, const QModelIndex& a_source_parent) const
    {
        QModelIndex name_index = sourceModel()->index(a_source_row, RecordColumn::Name, a_source_parent);
        QModelIndex opponent_name_index = sourceModel()->index(a_source_row, RecordColumn::OpponentName, a_source_parent);
        QString name = sourceModel()->data(name_index).toString();
        QString opponent_name = sourceModel()->data(opponent_name_index).toString();
        return name_pattern.match(name).hasMatch() && opponent_name_pattern.match(opponent_name).hasMatch();
    }
    
    bool RecordSortFilterProxyModel::pointsInRange(int a_source_row, const QModelIndex& a_source_parent) const
    {
        QModelIndex points_index = sourceModel()->index(a_source_row, RecordColumn::Points, a_source_parent);
        QModelIndex opponent_points_index = sourceModel()->index(a_source_row, RecordColumn::OpponentPoints, a_source_parent);

        int points = sourceModel()->data(points_index).toInt();
        int opponent_points = sourceModel()->data(opponent_points_index).toInt();

        if (minimum_points_enabled && (points < minimum_points))
        {
            return false;
        }

        if (maximum_points_enabled && (points > maximum_points))
        {
            return false;
        }

        if (minimum_opponent_points_enabled && (opponent_points < minimum_opponent_points))
        {
            return false;
        }

        if (maximum_opponent_points_enabled && (opponent_points > maximum_opponent_points))
        {
            return false;
        }

        return true;
    }
}