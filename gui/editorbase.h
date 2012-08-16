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


#ifndef EDITORBASE_H
#define EDITORBASE_H

#include <QGraphicsScene>

#include "gui/itemeditorwidgetsbase.h"

class Scene;


/** \brief  Base class for item based editors

        All item based editors (these which appear in center region of the main window) must subclass this

**/
class EditorBase : public QGraphicsScene
{
    Q_OBJECT
public:
    EditorBase(Scene *scene);

protected:
    Scene *m_scene;
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
signals:
    void itemClicked(ItemEditorWidgetsBase::editMessage msg);
};

#endif // EDITORBASE_H
