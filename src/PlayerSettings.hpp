#ifndef __PLAYERSETTINGS_H__
#define __PLAYERSETTINGS_H__

#include <ranges>
#include <vector>

#include <QString>

#include "PlayerType.hpp"

namespace wf
{
    class PlayerSettings
    {
        public:
            PlayerSettings(QString a_name, QString a_type);
            PlayerSettings(QString a_name, PlayerType a_type);
            PlayerSettings();
            ~PlayerSettings();

            const QString getName() const;
            void setName(QString a_name);
            const QString getTypeAsString() const;
            void setTypeWithString(QString a_type);
            PlayerType getType() const;
            void setType(PlayerType a_type);
            const std::vector<std::pair<PlayerType, QString>>& getAllTypes() const;
        
        private:
            QString name;
            PlayerType type;

            const std::vector<std::pair<PlayerType, QString>> all_types =
            {
                {PlayerType::AI, "AI"},
                {PlayerType::Human, "Human"}
            };
    };
}

#endif // __PLAYERSETTINGS_H__