#include "Score.hpp"


namespace wf
{
    bool Score::sortScoreByPoints(const Score& a_first, const Score& a_second)
    {
        return a_first.points > a_second.points;
    }
    
    bool Score::sortControlDescription(const QString& a_first, const QString& a_second)
    {
        std::array<QString, 11> values =
        {
            "Human",
            "AI 100%",
            "AI 90%",
            "AI 80%",
            "AI 70%",
            "AI 60%",
            "AI 50%",
            "AI 40%",
            "AI 30%",
            "AI 20%",
            "AI 10%"
        };

        size_t first_index = values.size();
        size_t second_index = values.size();

        for (size_t index = 0; index < values.size(); ++index)
        {
            if (a_first == values[index])
            {
                first_index = index;
            }

            if (a_second == values[index])
            {
                second_index = index;
            }
        }

        return first_index < second_index;
    }
    
    QString Score::getControlDescription(PlayerType a_player_type, int a_difficulty)
    {
        switch (a_player_type)
        {
            case PlayerType::AI:
            {
                return QString() % "AI " % QString::number(a_difficulty * 10) % "%";
            }
            case PlayerType::Human:
            {
                return "Human";
            }
        }

        return "Error";
    }
    
    bool Score::sameGameSetup(const Score& a_first, const Score& a_second)
    {
        return
            std::tie
            (
                a_first.name,
                a_first.player_type,
                a_first.difficulty,
                a_first.opponent_player_type,
                a_first.opponent_difficulty,
                a_first.dictionary,
                a_first.modifier_pattern
            )
            ==
            std::tie
            (
                a_second.name,
                a_second.player_type,
                a_second.difficulty,
                a_second.opponent_player_type,
                a_second.opponent_difficulty,
                a_second.dictionary,
                a_second.modifier_pattern
            );
    }
    
    bool ScoreControlCompare::operator()(const QString& a_first, const QString& a_second) const
    {
        return Score::sortControlDescription(a_first, a_second);
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