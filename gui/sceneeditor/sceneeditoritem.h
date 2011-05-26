#ifndef SCENEEDITORITEM_H
#define SCENEEDITORITEM_H

#include "gui/editoritem.h"
#include "gui/editorlabel.h"
#include "core/brainiacglobals.h"

class SceneEditorItem : public EditorItem
{
public:
    SceneEditorItem(BrainiacGlobals::ItemType type, void *object);
};

#endif // SCENEEDITORITEM_H
