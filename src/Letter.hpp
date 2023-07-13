#ifndef __LETTER_H__
#define __LETTER_H__

#include <utility>

#include <QString>
#include <QLabel>
#include <QPixmap>

#include "ForwardDeclarations.hpp"

#include "LetterType.hpp"
#include "LetterStatus.hpp"

namespace wf
{
    struct LetterData
    {
        QString letter;
        int count;
        int points;
    };
    
    class Letter
    {
        public:
            Letter();
            Letter(QString a_letter, int a_points);
            ~Letter();

            void setText(QString a_text);
            QString getText() const;
            void setPoints(int a_points);
            QString getPointsAsText() const;
            int getPoints() const;
            LetterType getType() const;
            void setStatus(LetterStatus a_status);
            LetterStatus getStatus() const;
            void setWildcardText(QString a_text);
            QString getWildcardText() const;
            void setOwner(Player* a_player);
            Player* getOwner();

        private:
            QString text;
            QString wildcard_text;
            int points = 0;
            LetterType type;
            LetterStatus status = LetterStatus::Free;
            Player* owner;
    };
}
#endif // __LETTER_H__