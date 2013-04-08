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


#include "bodyeditor.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/body.h"
#include "core/agent/body/bodymanager.h"
#include "core/agent/body/segment.h"
#include "core/brainiacglobals.h"
#include "gui/bodyeditor/bodyeditoritem.h"
#include "gui/editoritemconnector.h"

#include <QGraphicsSceneMouseEvent>

BodyEditor::BodyEditor(Scene *scene, AgentManager *agentManager) : EditorBase(scene)
{
    m_agentManager=agentManager;
    QHash<quint32, BodyEditorItem*> segItems;
    foreach(Segment *seg,m_agentManager->getBodyManager()->getSegments()) {
        BodyEditorItem *item=0;
        switch(seg->getType()) {
        case BrainiacGlobals::SPHERE:
            item=new BodyEditorItem(BrainiacGlobals::SPHERE,m_agentManager,seg->getId());
            item->setPos(agentManager->getEditorSegmentNodeLocations().value(seg->getId()).x(),agentManager->getEditorSegmentNodeLocations().value(seg->getId()).y());
            addItem(item);
            break;
        case BrainiacGlobals::TUBE:
            item=new BodyEditorItem(BrainiacGlobals::TUBE,m_agentManager,seg->getId());
            item->setPos(agentManager->getEditorSegmentNodeLocations().value(seg->getId()).x(),agentManager->getEditorSegmentNodeLocations().value(seg->getId()).y());
            addItem(item);
            break;
        case BrainiacGlobals::CUBE:
            item=new BodyEditorItem(BrainiacGlobals::CUBE,m_agentManager,seg->getId());
            item->setPos(agentManager->getEditorSegmentNodeLocations().value(seg->getId()).x(),agentManager->getEditorSegmentNodeLocations().value(seg->getId()).y());
            addItem(item);
        default:
            qCritical() << __PRETTY_FUNCTION__ << "seg Type not handled" << seg->getType();
        }
        segItems.insert(seg->getId(),item);
    }
    foreach(Segment *seg,m_agentManager->getBodyManager()->getSegments()) {
        if(seg->getParentId()) {
            BodyEditorItem *parentItem=segItems.value(seg->getParentId(),0);
            if(parentItem) {
                EditorItemConnector *connector=new EditorItemConnector(parentItem,segItems.value(seg->getId()));
                addItem(connector);
            }
        }
    }


    //! \bug  OK..Doing Here really nasty stuff. The Connectors are NOT SHOWING when scene is initially displayed So move all items one to the right and back again to schedule a redraw... Why the F*CK
    foreach (QGraphicsItem *item, items()) {
        if (item->type() == BodyEditorItem::Type) {
            BodyEditorItem *eItem=(BodyEditorItem *)item;
            eItem->setPos(eItem->x()+EditorItem::_raster,eItem->y());
            eItem->setPos(eItem->x()-EditorItem::_raster,eItem->y());
        }
    }
}

void BodyEditor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    BodyEditorItem *item=qgraphicsitem_cast<BodyEditorItem *>(itemAt( event->scenePos().x(), event->scenePos().y()) );
    ItemEditorWidgetsBase::editMessage msg;
    if( item ) {
        if(item->getType() == BrainiacGlobals::CUBE) {
            msg.object=item->getObject();
            msg.type=BrainiacGlobals::CUBE;
            msg.id=item->getId();
            emit itemClicked(msg);
        }
    }
}

void BodyEditor::updateItemLocations()
{
    foreach (QGraphicsItem *item, items()) {
        if (item->type() == BodyEditorItem::Type) {
            BodyEditorItem* bItem=(BodyEditorItem *) item;
            m_agentManager->setBodyEditorTranslation(bItem->getId(),(qint32)item->pos().x(),(qint32)item->pos().y());
        }
    }
}
