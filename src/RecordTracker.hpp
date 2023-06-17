#ifndef __RECORDTRACKER_H__
#define __RECORDTRACKER_H__

#include <algorithm>
#include <vector>

#include <QDataStream>
#include <QMetaType>
#include <QSettings>
#include <QString>
#include <QVector>

#include "ForwardDeclarations.hpp"

#include "PlayerType.hpp"

namespace wf
{
    struct Score
    {
        int points;
        QString name;
        PlayerType player_type;
        QString timestamp;
        QString result;
    };
    
    class RecordTracker : public QSettings
    {
        public:
            RecordTracker();
            ~RecordTracker();

            static bool sortScoreByPoints(Score a_left, Score a_right);
            static QString getDateTimeFormat();

            void insert(Score a_score);
            void setScores(std::vector<Score> a_leaderboard, PlayerType a_player_type);
            const std::vector<Score> getScores(PlayerType a_player_type);
        
        private:
            void load();
            void save();
            void sortAndTrim(QVector<Score>* a_leaderboard);

            int max_leaderboard_size = 50;
            QVector<Score> player_scores;
            QVector<Score> player_ai_scores;
    };
}

Q_DECLARE_METATYPE(wf::Score);

QDataStream& operator<<(QDataStream& a_out, const wf::Score& a_score);
QDataStream& operator>>(QDataStream& a_in, wf::Score& a_score);

#endif // __RECORDTRACKER_H__