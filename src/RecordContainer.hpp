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

            int getScoreCount(const Score& a_score) const;
            void load();
            void save();
    };
}

#endif // __RECORDCONTAINER_H__