#include "TileEvaluation.hpp"


namespace wf
{
    TileEvaluation::TileEvaluation()
    { }
    
    TileEvaluation::TileEvaluation(
        int a_minimum_viable_letter_count_horisontal,
        int a_minimum_viable_letter_count_vertical)
        : minimum_viable_letter_count_horisontal(a_minimum_viable_letter_count_horisontal)
        , minimum_viable_letter_count_vertical(a_minimum_viable_letter_count_vertical)
    { }
    
    TileEvaluation::~TileEvaluation()
    { }
    
    int TileEvaluation::getMinimumViableLetterCount(Direction a_direction)
    {
        switch (a_direction)
        {
            case Direction::Horisontal:
            {
                return minimum_viable_letter_count_horisontal;
            }
            case Direction::Vertical:
            {
                return minimum_viable_letter_count_vertical;
            }
        }

        return -1;
    }
    
    void TileEvaluation::setMinimumViableLetterCount(Direction a_direction, int a_value)
    {
        switch (a_direction)
        {
            case Direction::Horisontal:
            {
                minimum_viable_letter_count_horisontal = a_value;
                return;
            }
            case Direction::Vertical:
            {
                minimum_viable_letter_count_vertical = a_value;
                return;
            }
        }
    }
}