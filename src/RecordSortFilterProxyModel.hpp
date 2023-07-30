#ifndef __RECORDSORTFILTERPROXYMODEL_H__
#define __RECORDSORTFILTERPROXYMODEL_H__

#include <set>

#include <QModelIndex>
#include <QObject>
#include <QRegularExpression>
#include <QSortFilterProxyModel>
#include <QString>
#include <QVariant>

#include "ForwardDeclarations.hpp"

#include "RecordColumn.hpp"
#include "Score.hpp"

namespace wf
{
    class RecordSortFilterProxyModel : public QSortFilterProxyModel
    {
        Q_OBJECT

        public:
            RecordSortFilterProxyModel(QObject* a_parent);
            ~RecordSortFilterProxyModel();

            QVariant headerData(int a_section, Qt::Orientation a_orientation, int a_role = Qt::DisplayRole) const override;
        
        public slots:
            void updateControlFilter(std::set<QString, ScoreControlCompare> a_controls);
            void updateOpponentControlFilter(std::set<QString, ScoreControlCompare> a_opponent_controls);
            void updateNameFilter(const QString& a_name);
            void updateOpponentNameFilter(const QString& a_opponent_name);
            void updateDictionaryFilter(std::set<QString> a_dictionaries);
            void updateModifierFilter(std::set<QString> a_modifiers);
            void enablePointsFilters(
                bool a_minimum_points_enabled,
                bool a_maximum_points_enabled,
                bool a_minimum_opponent_points_enabled,
                bool a_maximum_opponent_points_enabled);
            void updatePointsFilterValues(
                int a_minimum_points,
                int a_maximum_points,
                int a_minimum_opponent_points,
                int a_maximum_opponent_points);
        
        protected:
            bool filterAcceptsRow(int a_source_row, const QModelIndex& a_source_parent) const override;
        
        private:
            bool controlValid(int a_source_row, const QModelIndex& a_source_parent) const;
            bool dictionaryValid(int a_source_row, const QModelIndex& a_source_parent) const;
            bool modifierValid(int a_source_row, const QModelIndex& a_source_parent) const;
            bool nameValid(int a_source_row, const QModelIndex& a_source_parent) const;
            bool pointsInRange(int a_source_row, const QModelIndex& a_source_parent) const;

            QRegularExpression name_pattern;
            QRegularExpression opponent_name_pattern;
            std::set<QString, ScoreControlCompare> controls;
            std::set<QString, ScoreControlCompare> opponent_controls;
            std::set<QString> dictionaries;
            std::set<QString> modifiers;

            bool minimum_points_enabled = false;
            bool maximum_points_enabled = false;
            bool minimum_opponent_points_enabled = false;
            bool maximum_opponent_points_enabled = false;

            int minimum_points;
            int maximum_points;
            int minimum_opponent_points;
            int maximum_opponent_points;
    };
}

#endif // __RECORDSORTFILTERPROXYMODEL_H__