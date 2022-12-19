#ifndef __LETTER_H__
#define __LETTER_H__

#include <QString>
#include <QLabel>
#include <QPixmap>

#include "ForwardDeclarations.hpp"

namespace wf
{
    struct LetterData
    {
        QString letter;
        int count;
        int points;
    };

    enum class LetterType
    {
        None,
        Letter,
        Wildcard
    };

    enum class LetterStatus
    {
        Unlocked,
        Locked
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
            void lock();
            LetterStatus getStatus();

        private:
            QString text;
            int points;
            LetterType type;
            LetterStatus status = LetterStatus::Unlocked;
    };
}
#endif // __LETTER_H__