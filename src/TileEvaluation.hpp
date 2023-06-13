#ifndef __TILEEVALUATION_H__
#define __TILEEVALUATION_H__

#include "Direction.hpp"

namespace wf
{
    class TileEvaluation
    {
        public:
            TileEvaluation();
            TileEvaluation(
                int a_minimum_viable_letter_count_horisontal,
                int a_minimum_viable_letter_count_vertical);
            ~TileEvaluation();

            int getMinimumViableLetterCount(Direction a_direction);
            void setMinimumViableLetterCount(Direction a_direction, int a_value);

        private:
            int minimum_viable_letter_count_horisontal;
            int minimum_viable_letter_count_vertical;
    };
}
#endif // __TILEEVALUATION_H__