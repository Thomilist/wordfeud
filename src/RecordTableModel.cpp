#include "RecordTableModel.hpp"


namespace wf
{
    RecordTableModel::RecordTableModel(QWidget* a_parent)
        : QAbstractTableModel(a_parent)
        , scores("Thomilist", "wordfeud-highscores", a_parent)
    {
        updatePointsLimits();
    }
    
    RecordTableModel::~RecordTableModel()
    { }
    
    QString RecordTableModel::getDateTimeFormat()
    {
        return "yyyy-MM-dd hh:mm:ss";
    }
    
    QString RecordTableModel::playerTypeAsText(Score a_score, bool a_opponent)
    {
        PlayerType type = PlayerType::Human;
        int difficulty = 0;

        if (a_opponent)
        {
            type = a_score.opponent_player_type;
            difficulty = a_score.opponent_difficulty;
        }
        else
        {
            type = a_score.player_type;
            difficulty = a_score.difficulty;
        }
        
        switch (type)
        {
            case PlayerType::AI:
            {
                return QString() % "AI " % QString::number(difficulty * 10) % "%";
            }
            case PlayerType::Human:
            {
                return "Human";
            }
        }

        return "Error";
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
        // timestamp, modifier_pattern, dictionary
        return 3+1+3+1+3;
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
                    case 0:
                    {
                        return scores[a_index.row()].name;
                    }
                    case 1:
                    {
                        return playerTypeAsText(scores[a_index.row()]);
                    }
                    case 2:
                    {
                        return scores[a_index.row()].points;
                    }
                    case 3:
                    {
                        return "vs.";
                    }
                    case 4:
                    {
                        return scores[a_index.row()].opponent_points;
                    }
                    case 5:
                    {
                        return playerTypeAsText(scores[a_index.row()], true);
                    }
                    case 6:
                    {
                        return scores[a_index.row()].opponent_name;
                    }
                    case 7:
                    {
                        return QVariant();
                    }
                    case 8:
                    {
                        return scores[a_index.row()].dictionary;
                    }
                    case 9:
                    {
                        return scores[a_index.row()].modifier_pattern;
                    }
                    case 10:
                    {
                        return scores[a_index.row()].timestamp;
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
                            case 0:
                            {
                                return "Name";
                            }
                            case 1:
                            {
                                return "Control";
                            }
                            case 2:
                            {
                                return "Points";
                            }
                            case 3:
                            {
                                return QVariant();
                            }
                            case 4:
                            {
                                return "Opponent\nPoints";
                            }
                            case 5:
                            {
                                return "Opponent\nControl";
                            }
                            case 6:
                            {
                                return "Opponent\nName";
                            }
                            case 7:
                            {
                                return QVariant();
                            }
                            case 8:
                            {
                                return "Dictionary";
                            }
                            case 9:
                            {
                                return "Modifiers";
                            }
                            case 10:
                            {
                                return "Date";
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
        scores.addToScoreCount(a_score.player_type, 1);
        
        insertRow(rowCount());
        scores.back() = a_score;
        std::sort(scores.begin(), scores.end(), Score::sortScoreByPoints);

        trimRecords();
        updatePointsLimits();
        
        emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount() - 1), {Qt::DisplayRole});
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
    
    void RecordTableModel::trimRecords()
    {
        for (const auto& [type, count] : {std::pair{PlayerType::AI, PlayerType::Human}})
        {
            while (scores.getScoreCount(type) > maximum_leaderboard_size)
            {
                for (int index = scores.size() - 1; index >= 0; --index)
                {
                    if (scores[index].player_type == type)
                    {
                        removeRow(index);
                        scores.addToScoreCount(type, -1);

                        break;
                    }
                }
            }
        }
    }
    
    void RecordTableModel::updatePointsLimits()
    {
        if (scores.empty())
        {
            minimum_points = 0;
            maximum_points = 0;
            minimum_opponent_points = 0;
            maximum_opponent_points = 0;
        }
        else
        {
            int starting_value = scores[0].points;
            int opponent_starting_value = scores[0].opponent_points;

            minimum_points = starting_value;
            maximum_points = starting_value;
            minimum_opponent_points = opponent_starting_value;
            maximum_opponent_points = opponent_starting_value;
        }
        
        for (const auto& score : scores)
        {
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
        }

        emit pointsLimitsChanged(minimum_points, maximum_points, minimum_opponent_points, maximum_opponent_points);

        return;
    }
}