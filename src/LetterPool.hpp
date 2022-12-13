#ifndef __LETTERPOOL_H__
#define __LETTERPOOL_H__

#include <vector>
#include <random>

#include "ForwardDeclarations.hpp"
#include "Letter.hpp"
#include "Language.hpp"

namespace wf
{
    class LetterPool
    {
        public:
            LetterPool();
            ~LetterPool();

            void set(std::vector<Letter*> a_letter_pointers);
            [[nodiscard]] Letter* getRandomLetter();
            int getRemainingCount();
        
        private:
            std::vector<Letter*> pool;
    };
}
#endif // __LETTERPOOL_H__