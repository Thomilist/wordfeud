#include "Letter.hpp"
namespace wf
{
    bool operator<(const LetterData& a_first, const LetterData& a_second)
    {
        return std::tie(a_first.letter, a_first.count, a_first.points) < std::tie(a_second.letter, a_second.count, a_second.points);
    }

    Letter::Letter()
    {
        type = LetterType::None;
    }

    Letter::Letter(QString a_letter, int a_points)
        : text(a_letter)
        , points(a_points)
    {
        if (a_letter.length() > 0)
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
    
    void Letter::setText(QString a_text)
    {
        text = a_text;
        return;
    }
    
    QString Letter::getText() const
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
    
    void Letter::setPoints(int a_points)
    {
        points = a_points;
        return;
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