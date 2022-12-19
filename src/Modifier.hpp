#ifndef __MODIFIER_H__
#define __MODIFIER_H__

#include <QString>

#include "ForwardDeclarations.hpp"

namespace wf
{
    enum class ModifierType
    {
        None,
        Start,
        DoubleLetter,
        TripleLetter,
        DoubleWord,
        TripleWord
    };

    class Modifier
    {
        public:
            Modifier(ModifierType a_type);
            Modifier();
            ~Modifier();

            void setType(ModifierType a_type);
            ModifierType getType();
            QString getText();

        private:
            ModifierType type = ModifierType::None;
            QString text;
    };
}


#endif // __MODIFIER_H__