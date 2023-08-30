#include "Modifier.hpp"
namespace wf
{
    Modifier::Modifier(
        ModifierType a_type,
        const QString& a_text,
        int a_multiplier,
        const QColor& a_color)
    {
        setType(a_type);
        setText(a_text);
        setMultiplier(a_multiplier);
        setColor(a_color);
    }

    Modifier::Modifier()
    { }
    
    Modifier::~Modifier()
    { }
    
    const QString Modifier::getTypeAsText(ModifierType a_type)
    {
        switch (a_type)
        {
            case ModifierType::None:
            {
                return "None";
            }
            case ModifierType::Start:
            {
                return "Start";
            }
            case ModifierType::Letter:
            {
                return "Letter";
            }
            case ModifierType::Word:
            {
                return "Word";
            }
        }

        return "None";
    }
    
    ModifierType Modifier::getTypeFromText(const QString& a_type)
    {
        for (const auto& [type_text, type] :
        {
            std::pair{QString{"None"}, ModifierType::None},
            std::pair{QString{"Start"}, ModifierType::Start},
            std::pair{QString{"Letter"}, ModifierType::Letter},
            std::pair{QString{"Word"}, ModifierType::Word}
        })
        {
            if (a_type == type_text)
            {
                return type;
            }
        }

        return ModifierType::None;
    }
    
    void Modifier::setType(ModifierType a_type)
    {
        type = a_type;
        return;
    }
    
    void Modifier::setType(const QString& a_type)
    {
        type = getTypeFromText(a_type);
        return;
    }
    
    ModifierType Modifier::getType() const
    {
        return type;
    }
    
    const QString Modifier::getTypeAsText() const
    {
        return getTypeAsText(type);
    }
    
    void Modifier::setText(const QString& a_text)
    {
        text = a_text;
        return;
    }
    
    const QString& Modifier::getText() const
    {
        return text;
    }
    
    void Modifier::setMultiplier(int a_multiplier)
    {
        multiplier = a_multiplier;
        return;
    }
    
    int Modifier::getMultiplier() const
    {
        return multiplier;
    }
    
    void Modifier::setColor(const QColor& a_color)
    {
        color = a_color;
        return;
    }
    
    const QColor& Modifier::getColor() const
    {
        return color;
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
    
    void to_json(nlohmann::json& a_json, const Modifier& a_modifier)
    {
        a_json = nlohmann::json
        {
            {"type", a_modifier.getTypeAsText().toStdString()},
            {"text", a_modifier.getText().toStdString()},
            {"multiplier", a_modifier.getMultiplier()},
            {"color", {a_modifier.getColor().red(), a_modifier.getColor().green(), a_modifier.getColor().blue(), a_modifier.getColor().alpha()}}
        };

        return;
    }
    
    void from_json(const nlohmann::json& a_json, Modifier& a_modifier)
    {
        std::string type;
        std::string text;
        int multiplier;
        int red;
        int blue;
        int green;
        int alpha;

        a_json.at("type").get_to(type);
        a_json.at("text").get_to(text);
        a_json.at("multiplier").get_to(multiplier);
        a_json.at("color").at(0).get_to(red);
        a_json.at("color").at(1).get_to(green);
        a_json.at("color").at(2).get_to(blue);
        a_json.at("color").at(3).get_to(alpha);
        
        a_modifier.setType(QString::fromStdString(type));
        a_modifier.setText(QString::fromStdString(text));
        a_modifier.setMultiplier(multiplier);
        a_modifier.setColor(QColor{red, green, blue, alpha});
        
        return;
    }
}