#ifndef __LETTERSTATUS_H__
#define __LETTERSTATUS_H__

namespace wf
{
    enum class LetterStatus
    {
        Free,
        Proposed,
        LockedRecently,
        Locked
    };
}

#endif // __LETTERSTATUS_H__