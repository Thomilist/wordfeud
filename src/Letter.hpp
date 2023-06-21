#ifndef __LETTER_H__
#define __LETTER_H__

#include <QString>
#include <QChar>
#include <QLabel>
#include <QPixmap>

#include "ForwardDeclarations.hpp"

#include "LetterType.hpp"
#include "LetterStatus.hpp"

namespace wf
{
    struct LetterData
    {
        QChar letter;
        int count;
        int points;
    };
    
    class Letter
    {
        public:
            Letter();
            Letter(
                QChar a_letter,
                int a_points);
            ~Letter();

            QChar getText() const;
            QString getPointsAsText() const;
            int getPoints() const;
            LetterType getType() const;
            void setStatus(LetterStatus a_status);
            LetterStatus getStatus() const;
            void setWildcardText(QChar a_text);
            QChar getWildcardText() const;
            void setOwner(Player* a_player);
            Player* getOwner();

        private:
            QChar text;
            QChar wildcard_text;
            int points;
            LetterType type;
            LetterStatus status = LetterStatus::Free;
            Player* owner;
    };
}
#endif // __LETTER_H__