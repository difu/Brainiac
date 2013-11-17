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


#include "sceneeditor.h"
#include "core/scene.h"
#include "core/group/group.h"
#include "core/brainiacglobals.h"
#include "gui/sceneeditor/sceneeditoritem.h"
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include "core/agent/agentmanager.h"

SceneEditor::SceneEditor(Scene *scene) : EditorBase(scene)
{
    refresh();
}

void SceneEditor::refresh()
{
    foreach(QGraphicsItem* item, this->items()) {
        this->removeItem(item);
        delete item;
    }

    foreach(Group *grp,m_scene->getGroups()) {
        //qDebug() << "Group in editor added" << grp->getId();
        SceneEditorItem *item=new SceneEditorItem(BrainiacGlobals::GROUP,grp);
        item->setPos(grp->getEditorTranslationX(),grp->getEditorTranslationY());
        //item->setFlag();
        addItem(item);
        if(grp->getAgentManager()) {
            AgentManager *manager=grp->getAgentManager();
            item=new SceneEditorItem(BrainiacGlobals::AGENT,manager);
            item->setPos(manager->getEditorTranslationX(),manager->getEditorTranslationY());
            addItem(item);
        }
    }
}

void SceneEditor::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    SceneEditorItem *item = qgraphicsitem_cast<SceneEditorItem *>(itemAt( event->scenePos().x(), event->scenePos().y(), QTransform()) );
    ItemEditorWidgetsBase::editMessage msg;
    if( item ) {
        if(item->getType() == BrainiacGlobals::GROUP) {
            msg.object=item->getObject();
            msg.type=BrainiacGlobals::GROUP;
            emit itemClicked(msg);
        }
    }
}
