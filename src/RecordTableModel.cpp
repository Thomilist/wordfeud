#include "RecordTableModel.hpp"


namespace wf
{
    RecordTableModel::RecordTableModel(QWidget* a_parent)
        : QAbstractTableModel(a_parent)
        , scores("Thomilist", "wordfeud-highscores", a_parent)
    {
        updateFilterData();
    }
    
    RecordTableModel::~RecordTableModel()
    { }
    
    const QString RecordTableModel::getDateTimeFormat()
    {
        return "yyyy-MM-dd hh:mm:ss";
    }
    
    int RecordTableModel::rowCount(const QModelIndex&) const 
    {
        return scores.size();
    }
    
    int RecordTableModel::columnCount(const QModelIndex&) const 
    {
        // name, player_type/difficulty, points
        // vs
        // opponent_name, opponent_player_type/opponent_difficulty, opponent_points
        // [spacer]
        // dictionary, modifier_pattern, timestamp
        // datetime (hidden)
        return 3+1+3+1+3 + 1;
    }
    
    bool RecordTableModel::insertRows(int a_row, int a_count, const QModelIndex& a_parent)
    {
        beginInsertRows(a_parent, a_row, a_row + a_count - 1);
        scores.insert(scores.begin() + a_row, a_count, Score());
        endInsertRows();

        return true;
    }
    
    bool RecordTableModel::removeRows(int a_row, int a_count, const QModelIndex& a_parent)
    {
        beginRemoveRows(a_parent, a_row, a_row + a_count - 1);
        scores.erase(scores.begin() + a_row, scores.begin() + a_row + a_count);
        endRemoveRows();

        return true;
    }
    
    QVariant RecordTableModel::data(const QModelIndex& a_index, int a_role) const 
    {
        if (static_cast<size_t>(a_index.row()) >= scores.size())
        {
            return QVariant();
        }
        
        switch (a_role)
        {
            case Qt::DisplayRole:
            {
                switch (a_index.column())
                {
                    case RecordColumn::Name:
                    {
                        return scores[a_index.row()].name;
                    }
                    case RecordColumn::Control:
                    {
                        return Score::getControlDescription(scores[a_index.row()].player_type, scores[a_index.row()].difficulty);
                    }
                    case RecordColumn::Points:
                    {
                        return scores[a_index.row()].points;
                    }
                    case RecordColumn::Versus:
                    {
                        return "vs.";
                    }
                    case RecordColumn::OpponentPoints:
                    {
                        return scores[a_index.row()].opponent_points;
                    }
                    case RecordColumn::OpponentControl:
                    {
                        return Score::getControlDescription(scores[a_index.row()].opponent_player_type, scores[a_index.row()].opponent_difficulty);
                    }
                    case RecordColumn::OpponentName:
                    {
                        return scores[a_index.row()].opponent_name;
                    }
                    case RecordColumn::Spacer:
                    {
                        return QVariant();
                    }
                    case RecordColumn::Dictionary:
                    {
                        return scores[a_index.row()].dictionary;
                    }
                    case RecordColumn::Modifiers:
                    {
                        return scores[a_index.row()].modifier_pattern;
                    }
                    case RecordColumn::Timestamp:
                    {
                        return scores[a_index.row()].timestamp;
                    }
                    case RecordColumn::DateTime:
                    {
                        return scores[a_index.row()].datetime;
                    }
                }

                break;
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
        
        return QVariant();
    }
    
    QVariant RecordTableModel::headerData(int a_section, Qt::Orientation a_orientation, int a_role) const 
    {
        switch (a_orientation)
        {
            case Qt::Orientation::Horizontal:
            {
                switch (a_role)
                {
                    case Qt::DisplayRole:
                    {
                        switch (a_section)
                        {
                            case RecordColumn::Name:
                            {
                                return "Name";
                            }
                            case RecordColumn::Control:
                            {
                                return "Control";
                            }
                            case RecordColumn::Points:
                            {
                                return "Points";
                            }
                            case RecordColumn::Versus:
                            {
                                return QVariant();
                            }
                            case RecordColumn::OpponentPoints:
                            {
                                return "Opponent\nPoints";
                            }
                            case RecordColumn::OpponentControl:
                            {
                                return "Opponent\nControl";
                            }
                            case RecordColumn::OpponentName:
                            {
                                return "Opponent\nName";
                            }
                            case RecordColumn::Spacer:
                            {
                                return QVariant();
                            }
                            case RecordColumn::Dictionary:
                            {
                                return "Dictionary";
                            }
                            case RecordColumn::Modifiers:
                            {
                                return "Modifiers";
                            }
                            case RecordColumn::Timestamp:
                            {
                                return "End Date";
                            }
                            case RecordColumn::DateTime:
                            {
                                return "Datetime";
                            }
                        }

                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                break;
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

                break;
            }
        }

        return QVariant();
    }
    
    void RecordTableModel::insert(Score a_score)
    {
        insertRow(rowCount());
        scores.back() = a_score;
        std::sort(scores.begin(), scores.end(), Score::sortScoreByPoints);

        trimRecords(a_score);
        scores.save();
        
        emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1), {Qt::DisplayRole});
        updateFilterData();
        return;
    }
    
    int RecordTableModel::getMinimumPoints(bool a_opponent)
    {
        return a_opponent ? minimum_opponent_points : minimum_points;
    }
    
    int RecordTableModel::getMaximumPoints(bool a_opponent)
    {
        return a_opponent ? maximum_opponent_points : maximum_points;
    }
    
    const std::set<QString, ScoreControlCompare>& RecordTableModel::getControlEntries() const
    {
        return control_entries;
    }
    
    const std::set<QString, ScoreControlCompare>& RecordTableModel::getOpponentControlEntries()
    {
        return opponent_control_entries;
    }
    
    const std::set<QString>& RecordTableModel::getDictionaries() const
    {
        return dictionaries;
    }
    
    const std::set<QString>& RecordTableModel::getModifiers() const
    {
        return modifiers;
    }
    
    const QDateTime& RecordTableModel::getEarliestDate() const
    {
        return earliest_datetime;
    }
    
    const QDateTime& RecordTableModel::getLatestDate() const
    {
        return latest_datetime;
    }
    
    void RecordTableModel::trimRecords(const Score& a_score)
    {
        while (scores.getScoreCount(a_score) > maximum_leaderboard_size)
        {
            for (int index = scores.size() - 1; index >= 0; --index)
            {
                if (Score::sameGameSetup(a_score, scores[index]))
                {
                    removeRow(index);
                    break;
                }
            }
        }
    }
    
    void RecordTableModel::updateFilterData()
    {
        control_entries.clear();
        opponent_control_entries.clear();
        dictionaries.clear();
        modifiers.clear();

        if (scores.empty())
        {
            minimum_points = 0;
            maximum_points = 0;
            minimum_opponent_points = 0;
            maximum_opponent_points = 0;
            earliest_datetime = QDateTime::currentDateTime();
            latest_datetime = earliest_datetime;
        }
        else
        {
            int starting_value = scores[0].points;
            int opponent_starting_value = scores[0].opponent_points;

            minimum_points = starting_value;
            maximum_points = starting_value;
            minimum_opponent_points = opponent_starting_value;
            maximum_opponent_points = opponent_starting_value;

            QDateTime starting_datetime = scores[0].datetime;
            earliest_datetime = starting_datetime;
            latest_datetime = starting_datetime;
        }

        for (const auto& score : scores)
        {
            control_entries.insert(Score::getControlDescription(score.player_type, score.difficulty));
            opponent_control_entries.insert(Score::getControlDescription(score.opponent_player_type, score.opponent_difficulty));
            dictionaries.insert(score.dictionary);
            modifiers.insert(score.modifier_pattern);

            if (score.points < minimum_points)
            {
                minimum_points = score.points;
            }
            else if (score.points > maximum_points)
            {
                maximum_points = score.points;
            }

            if (score.opponent_points < minimum_opponent_points)
            {
                minimum_opponent_points = score.opponent_points;
            }
            else if (score.opponent_points > maximum_opponent_points)
            {
                maximum_opponent_points = score.opponent_points;
            }

            if (score.datetime < earliest_datetime)
            {
                earliest_datetime = score.datetime;
            }
            else if (score.datetime > latest_datetime)
            {
                latest_datetime = score.datetime;
            }
        }

        emit filterDataUpdated();

        return;
    }
}