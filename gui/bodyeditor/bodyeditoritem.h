#ifndef BODYEDITORITEM_H
#define BODYEDITORITEM_H

#include "gui/editoritem.h"
#include "gui/editorlabel.h"
#include "core/brainiacglobals.h"

class BodyEditorItem : public EditorItem
{
public:
    BodyEditorItem(BrainiacGlobals::ItemType type, void *object,quint32 id);
};

#endif // BODYEDITORITEM_H
