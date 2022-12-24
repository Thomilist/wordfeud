#include "Modifier.hpp"
namespace wf
{
    Modifier::Modifier(ModifierType a_type)
    {
        setType(a_type);
    }

    Modifier::Modifier()
    { }
    
    Modifier::~Modifier()
    { }
    
    void Modifier::setType(ModifierType a_type)
    {
        type = a_type;

        switch (type)
        {
            case ModifierType::DoubleLetter:
            {
                text = "DL";
                break;
            }
            case ModifierType::DoubleWord:
            {
                text = "DW";
                break;
            }
            case ModifierType::TripleLetter:
            {
                text = "TL";
                break;
            }
            case ModifierType::TripleWord:
            {
                text = "TW";
                break;
            }
            default:
            {
                text = "";
                break;
            }
        }

        return;
    }
    
    ModifierType Modifier::getType() const
    {
        return type;
    }
    
    QString Modifier::getText() const
    {
        return text;
    }
    
    void Modifier::setUsed(bool a_used)
    {
        used = a_used;
        return;
    }
    
    bool Modifier::isUsed()
    {
        return used;
    }
}