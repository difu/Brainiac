// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


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
