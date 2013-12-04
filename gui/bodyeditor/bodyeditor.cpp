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

BodyEditor::BodyEditor(Scene *scene, AgentManager *agentManager) : m_agentManager(agentManager), EditorBase(scene)
{

}

void BodyEditor::addSegment(quint32 id)
{
    Segment seg=m_agentManager->getBodyManager()->getSegment(id);
    if(seg.getId()==0) {
        qCritical() << __PRETTY_FUNCTION__ << "Segment with id " << id << "does not exist!";
        return;
    }
    BodyEditorItem *newItem=0;
    newItem=new BodyEditorItem((BrainiacGlobals::ItemType)seg.getType(),m_agentManager,seg.getId());
    newItem->setPos(m_agentManager->getEditorSegmentNodeLocations().value(seg.getId()).x(),m_agentManager->getEditorSegmentNodeLocations().value(seg.getId()).y());
    addItem(newItem);
    // qDebug() << "Added id " << id << m_agentManager->getEditorSegmentNodeLocations().value(seg.getId()).x()  << m_agentManager->getEditorSegmentNodeLocations().value(seg.getId()).y();
    if(!seg.isRootSegment()) {
        quint32 parentId=seg.getParentId();
        foreach (QGraphicsItem *item, items()) {
            if (item->type() == BodyEditorItem::Type) {
                BodyEditorItem *eItem=(BodyEditorItem *)item;
                if(eItem->getId()==parentId) {
                    EditorItemConnector *connector=new EditorItemConnector(eItem,newItem);
                    addItem(connector);
                }
            }
        }

    }

    //! \bug  OK..Doing Here really nasty stuff. The Connectors are NOT SHOWING when scene is initially displayed. So move all items one to the right and back again to schedule a redraw... Why the F*CK
    foreach (QGraphicsItem *item, items()) {
        if (item->type() == BodyEditorItem::Type) {
            BodyEditorItem *eItem=(BodyEditorItem *)item;
            eItem->setPos(eItem->x()+EditorItem::_raster,eItem->y());
            eItem->setPos(eItem->x()-EditorItem::_raster,eItem->y());
        }
    }
}

void BodyEditor::autoArrange()
{
    Segment rootSeg=m_agentManager->getBodyManager()->getRootSegment();
    foreach(quint32 childId, m_agentManager->getBodyManager()->getChildIds(rootSeg.getId())) {
        autoArrangeRec(childId);
    }

}

void BodyEditor::autoArrangeRec(quint32 segId)
{
    Segment seg=m_agentManager->getBodyManager()->getSegment(segId);
    quint32 parentId=m_agentManager->getBodyManager()->getSegment(segId).getParentId();
    BodyEditorItem *parentItem=getItem(parentId);
    BodyEditorItem *myItem=getItem(segId);
    if(!parentItem)
        return;
    QPointF parentPos=parentItem->pos();
    qreal factor=1.3;
    switch(getDirection(seg.getRestTranslation())) {
    case BrainiacGlobals::NORTH:
        myItem->setPos(parentPos.x(),parentPos.y()-factor*EditorItem::HEIGHT);
        break;
    case BrainiacGlobals::SOUTH:
        myItem->setPos(parentPos.x(),parentPos.y()+factor*EditorItem::HEIGHT);
        break;
    case BrainiacGlobals::EAST:
        myItem->setPos(parentPos.x()-factor*EditorItem::WIDTH,parentPos.y());
        break;
    case BrainiacGlobals::WEST:
        myItem->setPos(parentPos.x()+factor*EditorItem::WIDTH,parentPos.y());
        break;
    }
    foreach(quint32 childId, m_agentManager->getBodyManager()->getChildIds(segId)) {
        autoArrangeRec(childId);
    }
}

BodyEditorItem* BodyEditor::getItem(quint32 id)
{
    foreach (QGraphicsItem *item, items()) {
        if (item->type() == BodyEditorItem::Type) {
            BodyEditorItem *eItem=(BodyEditorItem *)item;
            if( eItem->getId()==id) {
                return eItem;
            }
        }
    }
    return 0;
}

BrainiacGlobals::Direction BodyEditor::getDirection(const QVector3D &vec) const
{
    qreal y=vec.y();
    qreal minXZ=qMin(vec.x(),vec.z());
    qreal maxXZ=qMax(vec.x(),vec.z());
    if(y>=0 && y>= maxXZ) {
        return BrainiacGlobals::NORTH;
    } else if (y<0 && y<minXZ) {
        return BrainiacGlobals::SOUTH;
    } else if (qAbs(minXZ)>maxXZ && minXZ<0) {
        return BrainiacGlobals::WEST;
    } else {
        return BrainiacGlobals::EAST;
    }
}
void BodyEditor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    BodyEditorItem *item=qgraphicsitem_cast<BodyEditorItem *>(itemAt( event->scenePos().x(), event->scenePos().y(),QTransform()) );
    ItemEditorWidgetsBase::editMessage msg;
    if( item ) {
        if(item->getType() == BrainiacGlobals::CUBE) {
            msg.object=item->getObject();
            msg.type=BrainiacGlobals::CUBE;
            msg.id=item->getId();
            emit itemClicked(msg);
        } else if (item->getType() == BrainiacGlobals::TUBE) {
            msg.object=item->getObject();
            msg.type=BrainiacGlobals::TUBE;
            msg.id=item->getId();
            emit itemClicked(msg);
        } else {
            qDebug() << __PRETTY_FUNCTION__ << "Item type " << item->getType() << "not yet implemented";
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
