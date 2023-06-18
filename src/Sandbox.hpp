#ifndef __SANDBOX_H__
#define __SANDBOX_H__

#include <vector>

#include <QChar>

#include "Letter.hpp"
#include "VirtualBoard.hpp"

namespace wf
{
    struct Sandbox
    {
        VirtualBoard board;
        VirtualBoard best_play;
        std::vector<QChar> best_play_wildcard_letters;
        std::vector<Letter*> available_letters;
        int available_letter_count = 0;
        int touch_count = 0;
    };
}

#endif // __SANDBOX_H__