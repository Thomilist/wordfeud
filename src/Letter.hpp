#ifndef __LETTER_H__
#define __LETTER_H__

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

            QString getText();
            QString getPointsAsText();
            int getPoints();
            LetterType getType();
            void setStatus(LetterStatus a_status);
            LetterStatus getStatus();

        private:
            QString text;
            int points;
            LetterType type;
            LetterStatus status = LetterStatus::Free;
    };
}
#endif // __LETTER_H__