#ifndef __PLAYERSETTINGS_H__
#define __PLAYERSETTINGS_H__

#include <random>
#include <ranges>
#include <set>
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

            void apply();
            const QString getName() const;
            const QString getTempName() const;
            void setName(QString a_name);
            const QString getTypeAsString() const;
            const QString getTempTypeAsString() const;
            void setTypeWithString(QString a_type);
            PlayerType getType() const;
            PlayerType getTempType() const;
            void setType(PlayerType a_type);
            const std::vector<std::pair<PlayerType, QString>>& getAllTypes() const;
            bool usesRandomName() const;
            bool usesRandomNameTemp() const;
            void setRandomNameUse(bool a_state);
            void setRandomNames(std::set<QString> a_names);
            int getAIDifficulty() const;
            int getTempAIDifficulty() const;
            void setAIDifficulty(int a_difficulty);
        
        private:
            const QString getRandomName() const;
            
            // User-facing settings
            QString name;
            PlayerType type;
            bool use_random_name;
            int difficulty;

            // Temporary user-facing settings
            QString name_temp;
            PlayerType type_temp;
            bool use_random_name_temp;
            int difficulty_temp;

            std::vector<QString> random_names;

            const std::vector<std::pair<PlayerType, QString>> all_types =
            {
                {PlayerType::AI, "AI"},
                {PlayerType::Human, "Human"}
            };
    };
}

#endif // __PLAYERSETTINGS_H__