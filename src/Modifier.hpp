#ifndef __MODIFIER_H__
#define __MODIFIER_H__

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
            Modifier();
            ~Modifier();

        private:
            ModifierType type = ModifierType::None;
    };
}


#endif // __MODIFIER_H__