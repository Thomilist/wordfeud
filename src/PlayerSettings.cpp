#include "PlayerSettings.hpp"


namespace wf
{
    PlayerSettings::PlayerSettings(QString a_name, QString a_type)
        : name(a_name)
    {
        setTypeWithString(a_type);
    }
    
    PlayerSettings::PlayerSettings(QString a_name, PlayerType a_type)
        : name(a_name)
        , type(a_type)
    { }

    PlayerSettings::PlayerSettings()
    { }
    
    PlayerSettings::~PlayerSettings()
    { }
    
    void PlayerSettings::apply()
    {
        name = name_temp;
        type = type_temp;
        use_random_name = use_random_name_temp;
        difficulty = difficulty_temp;

        return;
    }
    
    const QString PlayerSettings::getName() const
    {
        if (usesRandomName())
        {
            return getRandomName();
        }
        else
        {
            return name;
        }
    }
    
    const QString PlayerSettings::getTempName() const
    {
        return name_temp;
    }
    
    void PlayerSettings::setName(QString a_name)
    {
        name_temp = a_name;
        return;
    }
    
    const QString PlayerSettings::getTypeAsString() const
    {
        auto iterator = std::ranges::find(all_types, type, &std::pair<PlayerType, QString>::first);
        return iterator->second;
    }
    
    void PlayerSettings::setTypeWithString(QString a_type)
    {
        auto iterator = std::ranges::find(all_types, a_type, &std::pair<PlayerType, QString>::second);
        setType(iterator->first);
        return;
    }
    
    PlayerType PlayerSettings::getType() const
    {
        return type;
    }
    
    PlayerType PlayerSettings::getTempType() const
    {
        return type_temp;
    }
    
    void PlayerSettings::setType(PlayerType a_type)
    {
        type_temp = a_type;
        return;
    }
    
    const std::vector<std::pair<PlayerType, QString>>& PlayerSettings::getAllTypes() const
    {
        return all_types;
    }
    
    bool PlayerSettings::usesRandomName() const
    {
        return use_random_name;
    }
    
    bool PlayerSettings::usesRandomNameTemp() const
    {
        return use_random_name_temp;
    }
    
    void PlayerSettings::setRandomNameUse(bool a_state)
    {
        use_random_name_temp = a_state;
        return;
    }
    
    void PlayerSettings::setRandomNames(std::set<QString> a_names)
    {
        random_names.clear();
        random_names.assign(a_names.begin(), a_names.end());
        return;
    }
    
    int PlayerSettings::getAIDifficulty() const
    {
        return difficulty;
    }
    
    int PlayerSettings::getTempAIDifficulty() const
    {
        return difficulty_temp;
    }
    
    void PlayerSettings::setAIDifficulty(int a_difficulty)
    {
        difficulty_temp = a_difficulty;
        return;
    }
    
    const QString PlayerSettings::getRandomName() const
    {
        std::random_device random_seed;
        std::mt19937 rng(random_seed());
        std::uniform_int_distribution<> distribution(0, random_names.size() - 1);

        int random_index = distribution(rng);
        return random_names.at(random_index);
    }
}