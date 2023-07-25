#include "Score.hpp"


namespace wf
{
    bool Score::sortScoreByPoints(const Score& a_first, const Score& a_second)
    {
        return a_first.points > a_second.points;
    }

    bool operator<(const Score& a_first, const Score& a_second)
    {
        return a_first.points < a_second.points;
    }
}

QDataStream& operator<<(QDataStream& a_out, const wf::Score& a_score)
{
    a_out
        << a_score.name
        << a_score.player_type
        << a_score.difficulty
        << a_score.points

        << a_score.opponent_name
        << a_score.opponent_player_type
        << a_score.opponent_difficulty
        << a_score.opponent_points

        << a_score.dictionary
        << a_score.modifier_pattern
        << a_score.timestamp
        << a_score.datetime;

    return a_out;
}

QDataStream& operator>>(QDataStream& a_in, wf::Score& a_score)
{
    a_in >> a_score.name;
    a_in >> a_score.player_type;
    a_in >> a_score.difficulty;
    a_in >> a_score.points;

    a_in >> a_score.opponent_name;
    a_in >> a_score.opponent_player_type;
    a_in >> a_score.opponent_difficulty;
    a_in >> a_score.opponent_points;

    a_in >> a_score.dictionary;
    a_in >> a_score.modifier_pattern;
    a_in >> a_score.timestamp;
    a_in >> a_score.datetime;
    
    return a_in;
}