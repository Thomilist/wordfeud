#ifndef __DICTIONARYEDITORMODE_H__
#define __DICTIONARYEDITORMODE_H__

namespace wf
{
    enum class DictionaryEditorMode
    {
        Undefined,
        CreateNew,
        OpenCopy,
        EditExisting,
        ExportExisting,
        DeleteExisting
    };
}

#endif // __DICTIONARYEDITORMODE_H__