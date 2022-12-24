#ifndef __MODIFIER_H__
#define __MODIFIER_H__

#include <QString>

#include "ForwardDeclarations.hpp"
#include "ModifierType.hpp"

namespace wf
{
    class Modifier
    {
        public:
            Modifier(ModifierType a_type);
            Modifier();
            ~Modifier();

            void setType(ModifierType a_type);
            ModifierType getType() const;
            QString getText() const;
            void setUsed(bool a_used);
            bool isUsed();

        private:
            ModifierType type = ModifierType::None;
            QString text;
            bool used = false;
    };
}


#endif // __MODIFIER_H__