#ifndef __RECORDTABLEMODEL_H__
#define __RECORDTABLEMODEL_H__

#include <algorithm>
#include <set>
#include <vector>

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QSettings>
#include <QString>
#include <QStringBuilder>
#include <QVariant>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "RecordContainer.hpp"
#include "Score.hpp"

#include "PlayerType.hpp"
#include "RecordColumn.hpp"

namespace wf
{
    class RecordTableModel : public QAbstractTableModel
    {
        Q_OBJECT
        
        public:
            RecordTableModel(QWidget* a_parent = nullptr);
            ~RecordTableModel();

            static QString getDateTimeFormat();

            int rowCount(const QModelIndex& a_parent = QModelIndex()) const override;
            int columnCount(const QModelIndex& a_parent = QModelIndex()) const override;
            bool insertRows(int a_row, int a_count, const QModelIndex& a_parent = QModelIndex()) override;
            bool removeRows(int a_row, int a_count, const QModelIndex& a_parent = QModelIndex()) override;
            QVariant data(const QModelIndex& a_index, int a_role = Qt::DisplayRole) const override;
            QVariant headerData(int a_section, Qt::Orientation a_orientation, int a_role = Qt::DisplayRole) const override;

            void insert(Score a_score);
            int getMinimumPoints(bool a_opponent = false);
            int getMaximumPoints(bool a_opponent = false);
            const std::set<QString, ScoreControlCompare>& getControlEntries() const;
            const std::set<QString, ScoreControlCompare>& getOpponentControlEntries();
            const std::set<QString>& getDictionaries() const;
            const std::set<QString>& getModifiers() const;
        
        signals:
            void pointsLimitsChanged(int a_minimum_points, int a_maximum_points, int a_opponent_minimum_points, int a_opponent_maximum_points);
        
        private:
            void trimRecords();
            void updateFilterData();
            void updateControlEntries();
            void updateDictionaries();
            void updateModifiers();
            void updatePointsLimits();

            const int maximum_leaderboard_size = 100;
            RecordContainer scores;

            std::set<QString, ScoreControlCompare> control_entries;
            std::set<QString, ScoreControlCompare> opponent_control_entries;
            std::set<QString> dictionaries;
            std::set<QString> modifiers;

            int minimum_points;
            int maximum_points;
            int minimum_opponent_points;
            int maximum_opponent_points;
    };
}

#endif // __RECORDTABLEMODEL_H__