#include "RecordTracker.hpp"


namespace wf
{
    RecordTracker::RecordTracker()
    {
        qRegisterMetaType<Score>("Score");
        qRegisterMetaTypeStreamOperators<Score>("Score");

        load();
    }
    
    RecordTracker::~RecordTracker()
    {
        save();
    }
    
    bool RecordTracker::sortScoreByPoints(Score a_left, Score a_right)
    {
        return a_left.points > a_right.points;
    }
    
    QString RecordTracker::getDateTimeFormat()
    {
        return "yyyy-MM-dd hh:mm:ss";
    }
    
    void RecordTracker::insert(Score a_score)
    {
        QVector<Score>* leaderboard = nullptr;
        
        switch (a_score.player_type)
        {
            case PlayerType::Human:
            {
                leaderboard = &player_scores;
                break;
            }
            case PlayerType::AI:
            {
                leaderboard = &player_ai_scores;
                break;
            }
        }

        leaderboard->push_back(a_score);
        sortAndTrim(leaderboard);

        return;
    }
    
    void RecordTracker::setScores(std::vector<Score> a_leaderboard, PlayerType a_player_type)
    {
        QVector leaderboard = QVector<Score>(a_leaderboard.begin(), a_leaderboard.end());
        
        sortAndTrim(&leaderboard);
        
        switch (a_player_type)
        {
            case PlayerType::Human:
            {
                player_scores = leaderboard;
                break;
            }
            case PlayerType::AI:
            {
                player_ai_scores = leaderboard;
                break;
            }
        }

        return;
    }
    
    const std::vector<Score> RecordTracker::getScores(PlayerType a_player_type)
    {
        std::vector<Score> leaderboard;
        
        switch (a_player_type)
        {
            case PlayerType::Human:
            {
                leaderboard = std::vector<Score>(player_scores.begin(), player_scores.end());
                break;
            }
            case PlayerType::AI:
            {
                leaderboard = std::vector<Score>(player_ai_scores.begin(), player_ai_scores.end());
                break;
            }
        }

        return leaderboard;
    }
    
    void RecordTracker::load()
    {
        // Load high scores
        beginGroup("scores");

        // Player scores
        int size = beginReadArray("player");
        player_scores.resize(size);

        for (int index = 0; index < size; ++index)
        {
            setArrayIndex(index);
            player_scores[index] = value("score").value<Score>();
        }

        endArray();

        // Player AI scores
        size = beginReadArray("ai");
        player_ai_scores.resize(size);

        for (int index = 0; index < size; ++index)
        {
            setArrayIndex(index);
            player_ai_scores[index] = value("score").value<Score>();
        }

        endArray();

        endGroup();

        return;
    }
    
    void RecordTracker::save()
    {
        // Save high scores
        beginGroup("scores");

        // Player scores
        int index = 0;
        beginWriteArray("player");

        for (auto score : player_scores)
        {
            setArrayIndex(index++);
            setValue("score", QVariant::fromValue(score));
        }

        endArray();

        // Player AI scores
        index = 0;
        beginWriteArray("ai");

        for (auto score : player_ai_scores)
        {
            setArrayIndex(index++);
            setValue("score", QVariant::fromValue(score));
        }

        endArray();

        endGroup();

        return;
    }
    
    void RecordTracker::sortAndTrim(QVector<Score>* a_leaderboard)
    {
        std::sort(a_leaderboard->begin(), a_leaderboard->end(), sortScoreByPoints);

        while (a_leaderboard->size() > max_leaderboard_size)
        {
            a_leaderboard->pop_back();
        }

        return;
    }
}

QDataStream& operator<<(QDataStream& a_out, const wf::Score& a_score)
{
    a_out
        << a_score.name
        << a_score.player_type
        << a_score.points
        << a_score.timestamp
        << a_score.result
        << a_score.modifier_pattern
        << a_score.dictionary
        << a_score.difficulty;
    return a_out;
}

QDataStream& operator>>(QDataStream& a_in, wf::Score& a_score)
{
    a_in >> a_score.name;
    a_in >> a_score.player_type;
    a_in >> a_score.points;
    a_in >> a_score.timestamp;
    a_in >> a_score.result;
    a_in >> a_score.modifier_pattern;
    a_in >> a_score.dictionary;
    a_in >> a_score.difficulty;
    return a_in;
}
