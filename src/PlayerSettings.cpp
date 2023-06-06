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
        return typeEnumToString.at(type);
    }
    
    void PlayerSettings::setTypeWithString(QString a_type)
    {
        setType(typeStringToEnum.at(a_type));
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
}