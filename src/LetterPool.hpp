#ifndef __LETTERPOOL_H__
#define __LETTERPOOL_H__

#include <vector>
#include <random>

#include <QObject>

#include "ForwardDeclarations.hpp"
#include "Letter.hpp"
#include "Language.hpp"

namespace wf
{
    class LetterPool : public QObject
    {
        Q_OBJECT

        public:
            LetterPool();
            ~LetterPool();

            void set(std::vector<Letter*> a_letter_pointers);
            [[nodiscard]] Letter* getRandomLetter();
            int getRemainingCount();
        
        signals:
            void remainingCountChanged(int a_count);

        private:
            std::vector<Letter*> pool;
    };
}
#endif // __LETTERPOOL_H__