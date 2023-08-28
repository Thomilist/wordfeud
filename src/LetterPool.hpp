#ifndef __LETTERPOOL_H__
#define __LETTERPOOL_H__

#include <map>
#include <random>
#include <set>
#include <vector>

#include <QObject>

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
            const std::set<QString>& getNonWildcardLetters() const;
            QString getNonWildcardLetterBefore(const QString& a_letter) const;
            QString getNonWildcardLetterAfter(const QString& a_letter) const;
            void insertLetter(Letter* a_letter);
            std::vector<Letter*> getWildcardSubstitutes(Letter* a_letter);
        
        signals:
            void remainingCountChanged(int a_count);

        private:
            std::vector<Letter> createWildcardSubstitutes();

            std::vector<Letter*> pool;
            std::set<QString> non_wildcard_letters;
            std::map<Letter*, std::vector<Letter>> wildcard_substitutes;
    };
}
#endif // __LETTERPOOL_H__