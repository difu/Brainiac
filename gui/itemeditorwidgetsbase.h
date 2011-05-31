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
    /** \brief  Message to communicate between item-based editors and the items properties editors

            It depends on the editors what properties to use, sometimes not all properties are used,
            the message receiver has to handle this

    **/
    struct editMessage {
        quint32 id; //!< usually the id of an item
        BrainiacGlobals::ItemType type; //!< the type of the item
        void *object; //!< free to use object
    };

protected:
    Scene *m_scene; //!< the project scene
};

#endif // ITEMEDITORWIDGETSBASE_H
