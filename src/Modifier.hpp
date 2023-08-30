#ifndef __MODIFIER_H__
#define __MODIFIER_H__

#include <string>

#include <QColor>
#include <QString>

#include <nlohmann/json.hpp>

#include "ForwardDeclarations.hpp"

#include "ModifierType.hpp"

namespace wf
{
    class Modifier
    {
        public:
            Modifier(
                ModifierType a_type,
                const QString& a_text,
                int a_multiplier,
                const QColor& a_color);
            Modifier();
            ~Modifier();

            static const QString getTypeAsText(ModifierType a_type);
            static ModifierType getTypeFromText(const QString& a_type);

            void setType(ModifierType a_type);
            void setType(const QString& a_type);
            ModifierType getType() const;
            const QString getTypeAsText() const;
            void setText(const QString& a_text);
            const QString& getText() const;
            void setMultiplier(int a_multiplier);
            int getMultiplier() const;
            void setColor(const QColor& a_color);
            const QColor& getColor() const;
            void setUsed(bool a_used);
            bool isUsed();

        private:
            ModifierType type = ModifierType::None;
            QString text;
            int multiplier = 1;
            QColor color;
            bool used = false;
    };

    void to_json(nlohmann::json& a_json, const Modifier& a_modifier);
    void from_json(const nlohmann::json& a_json, Modifier& a_modifier);
}


#endif // __MODIFIER_H__