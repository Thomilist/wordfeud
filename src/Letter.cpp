#include "Letter.hpp"
namespace wf
{
    Letter::Letter()
    {
        type = LetterType::None;
    }

    Letter::Letter(
        QChar a_letter,
        int a_points)
        : text(a_letter)
        , points(a_points)
    {
        if (a_letter.isLetter())
        {
            type = LetterType::Letter;
        }
        else
        {
            type = LetterType::Wildcard;
        }
    }
    
    Letter::~Letter()
    { }
    
    QChar Letter::getText() const
    {
        if (getType() == LetterType::Wildcard)
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
    
    void Letter::setWildcardText(QChar a_text)
    {
        wildcard_text = a_text;
        return;
    }
    
    QChar Letter::getWildcardText() const
    {
        return wildcard_text;
    }
    
    void Letter::setOwner(Player* a_player)
    {
        owner = a_player;
        return;
    }
    
    Player* Letter::getOwner()
    {
        return owner;
    }
}