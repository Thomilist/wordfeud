#ifndef __RECORDCONTAINER_H__
#define __RECORDCONTAINER_H__

#include <vector>

#include <QObject>
#include <QSettings>
#include <QString>
#include <QVariant>

#include "ForwardDeclarations.hpp"

#include "Score.hpp"

#include "PlayerType.hpp"

namespace wf
{
    class RecordContainer : public QSettings, public std::vector<Score>
    {
        public:
            RecordContainer(
                QString a_organisation,
                QString a_application,
                QObject* a_parent);
            ~RecordContainer();

            void addToScoreCount(PlayerType a_player_type, int a_count);
            int getScoreCount(PlayerType a_player_type) const;
        
        private:
            void load();
            void save();

            int ai_scores;
            int human_scores;
    };
}

#endif // __RECORDCONTAINER_H__