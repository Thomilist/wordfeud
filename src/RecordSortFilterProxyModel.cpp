#include "RecordSortFilterProxyModel.hpp"


namespace wf
{
    RecordSortFilterProxyModel::RecordSortFilterProxyModel(QObject* a_parent)
        : QSortFilterProxyModel(a_parent)
    {
        
    }
    
    RecordSortFilterProxyModel::~RecordSortFilterProxyModel()
    { }
    
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
        bool row_accepted = true;

        row_accepted &= pointsInRange(a_source_row, a_source_parent);

        return row_accepted;
    }
    
    bool RecordSortFilterProxyModel::pointsInRange(int a_source_row, const QModelIndex& a_source_parent) const
    {
        QModelIndex points_index = sourceModel()->index(a_source_row, 2, a_source_parent);
        QModelIndex opponent_points_index = sourceModel()->index(a_source_row, 4, a_source_parent);

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