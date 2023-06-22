#ifndef __LETTERPOOL_H__
#define __LETTERPOOL_H__

#include <random>
#include <vector>

#include <QObject>
#include <QStringList>

#include "ForwardDeclarations.hpp"

#include "Language.hpp"
#include "Letter.hpp"

namespace wf
{
    class LetterPool : public QObject
    {
        Q_OBJECT

        public:
            LetterPool();
            ~LetterPool();

            void set(std::vector<Letter*> a_letters);
            [[nodiscard]] Letter* getRandomLetter();
            int getRemainingCount() const;
            int getRemainingWildcardCount() const;
            QStringList getNonWildcardLetters() const;
            void insertLetter(Letter* a_letter);
        
        signals:
            void remainingCountChanged(int a_count);

        private:
            std::vector<Letter*> pool;
            QStringList non_wildcard_letters;
    };
}
#endif // __LETTERPOOL_H__