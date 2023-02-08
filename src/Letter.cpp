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
    
    QString Letter::getText() const
    {
        if (getType() == LetterType::Wildcard && wildcard_text.length() > 0)
        {
            return wildcard_text;
        }
        else
        {
            return text;
        }
    }
    
    QString Letter::getPointsAsText() const
    {
        return QString::number(points);
    }
    
    int Letter::getPoints() const
    {
        return points;
    }
    
    LetterType Letter::getType() const
    {
        return type;
    }
    
    void Letter::setStatus(LetterStatus a_status)
    {
        status = a_status;
        return;
    }
    
    LetterStatus Letter::getStatus() const
    {
        return status;
    }
    
    void Letter::setWildcardText(QString a_text)
    {
        wildcard_text = a_text;
        return;
    }
    
    QString Letter::getWildcardText() const
    {
        return wildcard_text;
    }
}