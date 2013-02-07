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
#include "core/agent/body/skeletonnode.h"
#include "core/agent/body/skeletonnodesphere.h"
#include "core/agent/body/skeletonnodebox.h"
#include "core/brainiacglobals.h"
#include "gui/bodyeditor/bodyeditoritem.h"
#include "gui/editoritemconnector.h"

#include <QGraphicsSceneMouseEvent>

BodyEditor::BodyEditor(Scene *scene, AgentManager *agentManager) : EditorBase(scene)
{
    m_agentManager=agentManager;
    foreach(SkeletonNode *n, agentManager->getMasterAgent()->getBody()->getAllSkeletonNodes() ) {
//        qDebug() << __PRETTY_FUNCTION__ << "added node "<< n->objectName();
//        qDebug() << __PRETTY_FUNCTION__ << agentManager->getEditorSkeletonNodeLocations().value(n->getId()).x();
        if(dynamic_cast<SkeletonNodeBox*>(n)) {
            BodyEditorItem *item=new BodyEditorItem(BrainiacGlobals::CUBE,m_agentManager,n->getId());
            item->setPos(agentManager->getEditorSkeletonNodeLocations().value(n->getId()).x(),agentManager->getEditorSkeletonNodeLocations().value(n->getId()).y());
            addItem(item);
        } else if(dynamic_cast<SkeletonNodeSphere*>(n)) {
            BodyEditorItem *item=new BodyEditorItem(BrainiacGlobals::SPHERE,m_agentManager,n->getId());
            item->setPos(agentManager->getEditorSkeletonNodeLocations().value(n->getId()).x(),agentManager->getEditorSkeletonNodeLocations().value(n->getId()).y());
            addItem(item);
        }
    }
    foreach (QGraphicsItem *item, items()) {
        if(item->type()==EditorItem::Type) {
            EditorItem *parentEditorItem = qgraphicsitem_cast<EditorItem*> (item);
            AgentManager *parentManager=(AgentManager *)parentEditorItem->getObject();
            foreach(QGLSceneNode *childNode, parentManager->getMasterAgent()->getBody()->getSkeletonNodeById(parentEditorItem->getId())->children()) {
                SkeletonNode *skelChildNode=dynamic_cast<SkeletonNode *>(childNode);
                if(skelChildNode) {
                    foreach (QGraphicsItem *childItem, items()) {
                        if(childItem->type()==EditorItem::Type) {
                            EditorItem *childEditorItem = qgraphicsitem_cast<EditorItem*> (childItem);
                            if(childEditorItem->getId() == skelChildNode->getId()) {
                                EditorItemConnector *connector=new EditorItemConnector(parentEditorItem,childEditorItem);
                                addItem(connector);
                            }
                        }
                    }
                }
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
