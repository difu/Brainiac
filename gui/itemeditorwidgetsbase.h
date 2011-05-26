#ifndef ITEMEDITORWIDGETSBASE_H
#define ITEMEDITORWIDGETSBASE_H
#include <QtCore>
#include "core/brainiacglobals.h"
class Scene;

/** \brief  Base class for all item editors

        All itemeditors (these which appear in south region of the main window) must subclass this

**/
class ItemEditorWidgetsBase
{
public:
    ItemEditorWidgetsBase(Scene *scene);
    struct editMessage {
        quint32 id;
        BrainiacGlobals::ItemType type;
        void *object;
    };

protected:
    Scene *m_scene;
};

#endif // ITEMEDITORWIDGETSBASE_H
