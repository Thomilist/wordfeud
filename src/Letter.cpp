#include "Letter.hpp"
namespace wf
{
    Letter::Letter()
    {
        type = LetterType::None;
    }

    Letter::Letter(QString a_letter, int a_points)
        : text(a_letter)
        , points(a_points)
    {
        if (a_letter.length() == 0)
        {
            type = LetterType::Wildcard;
        }
        else
        {
            type = LetterType::Letter;
        }
    }
    
    Letter::~Letter()
    { }
    
    QString Letter::getText()
    {
        return text;
    }
    
    int Letter::getPoints()
    {
        return points;
    }
    
    LetterType Letter::getType()
    {
        return type;
    }
    
    /*void Letter::draw(Tile* a_parent)
    {
        QLabel letter_background{a_parent};
        QPixmap letter_background_image{":/letter-background.png"};
        letter_background.setPixmap(letter_background_image.scaled(a_parent->size()));
    }*/
}