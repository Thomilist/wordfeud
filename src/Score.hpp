#ifndef __SCORE_H__
#define __SCORE_H__

#include <QDataStream>
#include <QDateTime>
#include <QMetaType>
#include <QString>

#include "ForwardDeclarations.hpp"

#include "PlayerType.hpp"

namespace wf
{
    struct Score
    {
        static bool sortScoreByPoints(const Score& a_first, const Score& a_second);
        
        QString name;
        PlayerType player_type;
        int difficulty;
        int points;

        QString opponent_name;
        PlayerType opponent_player_type;
        int opponent_difficulty;
        int opponent_points;

        QString dictionary;
        QString modifier_pattern;
        QString timestamp;
        QDateTime datetime;
    };

    bool operator<(const Score& a_first, const Score& a_second);
}

Q_DECLARE_METATYPE(wf::Score);

QDataStream& operator<<(QDataStream& a_out, const wf::Score& a_score);
QDataStream& operator>>(QDataStream& a_in, wf::Score& a_score);

#endif // __SCORE_H__