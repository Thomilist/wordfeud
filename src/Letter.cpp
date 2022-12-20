#include "Letter.hpp"
namespace wf
{
    Letter::Letter()
    {
        type = LetterType::None;
    }

    Letter::Letter(
        QString a_letter,
        int a_points)
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
    
    QString Letter::getPointsAsText()
    {
        return QString::number(points);
    }
    
    int Letter::getPoints()
    {
        return points;
    }
    
    LetterType Letter::getType()
    {
        return type;
    }
    
    void Letter::setStatus(LetterStatus a_status)
    {
        status = a_status;
        return;
    }
    
    LetterStatus Letter::getStatus()
    {
        return status;
    }
}