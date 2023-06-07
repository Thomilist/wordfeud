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
    
    const QString PlayerSettings::getName() const
    {
        return name;
    }
    
    void PlayerSettings::setName(QString a_name)
    {
        name = a_name;
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
    
    void PlayerSettings::setType(PlayerType a_type)
    {
        type = a_type;
        return;
    }
    
    const std::vector<std::pair<PlayerType, QString>>& PlayerSettings::getAllTypes() const
    {
        return all_types;
    }
}