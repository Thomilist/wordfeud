#include "Letter.hpp"
namespace wf
{
    Letter::Letter()
    { }

    Letter::Letter(QString a_letter, int a_points)
        : text(a_letter)
        , points(a_points)
    { }
    
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
}