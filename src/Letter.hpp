#ifndef __LETTER_H__
#define __LETTER_H__

#include <QString>

namespace wf
{
    class Letter
    {
        public:
            Letter();
            Letter(QString a_letter, int a_points);
            ~Letter();

            QString getText();
            int getPoints();

        private:
            QString text;
            int points;
    };
}
#endif // __LETTER_H__