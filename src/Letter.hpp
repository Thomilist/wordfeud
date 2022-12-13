#ifndef __LETTER_H__
#define __LETTER_H__

#include <QString>
#include <QLabel>
#include <QPixmap>

#include "ForwardDeclarations.hpp"
//#include "Tile.hpp"

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
    
    class Letter
    {
        public:
            Letter();
            Letter(QString a_letter, int a_points);
            ~Letter();

            QString getText();
            int getPoints();
            LetterType getType();
            //void draw(Tile* a_parent);

        private:
            QString text;
            int points;
            LetterType type;
    };
}
#endif // __LETTER_H__