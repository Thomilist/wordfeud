#ifndef __EDITORMODE_H__
#define __EDITORMODE_H__

namespace wf
{
    enum class EditorMode
    {
        Undefined,
        CreateNew,
        OpenCopy,
        EditExisting,
        ExportExisting,
        DeleteExisting
    };
}

#endif // __EDITORMODE_H__