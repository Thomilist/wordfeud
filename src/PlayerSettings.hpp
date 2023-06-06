#ifndef __PLAYERSETTINGS_H__
#define __PLAYERSETTINGS_H__

#include <map>

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
        
        private:
            QString name;
            PlayerType type;
            
            const std::map<PlayerType, QString> typeEnumToString =
            {
                {PlayerType::Human, "Human"},
                {PlayerType::AI,    "AI"}
            };

            const std::map<QString, PlayerType> typeStringToEnum =
            {
                {"Human",   PlayerType::Human},
                {"AI",      PlayerType::AI}
            };
    };
}

#endif // __PLAYERSETTINGS_H__