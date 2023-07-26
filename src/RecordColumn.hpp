#ifndef __RECORDCOLUMN_H__
#define __RECORDCOLUMN_H__

namespace wf
{
    enum RecordColumn : unsigned int
    {
        Name,
        Control,
        Points,
        Versus,
        OpponentPoints,
        OpponentControl,
        OpponentName,
        Spacer,
        Dictionary,
        Modifiers,
        Timestamp
    };
}

#endif // __RECORDCOLUMN_H__