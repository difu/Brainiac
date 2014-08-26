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


#include "braineditor.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/output.h"
#include "core/brainiaclogger.h"
#include "gui/braineditor/braineditoritem.h"
#include "gui/editoritemconnector.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>

BrainEditor::BrainEditor(Scene *scene, AgentManager *agentManager) : EditorBase(scene)
{
    m_agentManager=agentManager;
    m_altPressed=false;
    m_shiftPressed=false;
    m_connectSourceItem=0;
}

void BrainEditor::addFuzzyItem(quint32 id)
{
    if(m_agentManager->getMasterAgent()->getBrain()->getFuzzy(id)) {
        BrainEditorItem *item=new BrainEditorItem((BrainiacGlobals::ItemType)m_agentManager->getFuzzType(id),m_agentManager,id);
        item->setPos(m_agentManager->getEditorFuzzyLocations().value(id).x()+BrainEditorItem::WIDTH,m_agentManager->getEditorFuzzyLocations().value(id).y());
        addItem(item);
    } else {
        qCritical() << __PRETTY_FUNCTION__ << "No item with id"  << id;
    }

    //! \bug  OK..Doing Here really nasty stuff. The Connectors are NOT SHOWING when scene is initially displayed So move all items one to the right and back again to schedule a redraw... Why the F*CK
    foreach (QGraphicsItem *item, items()) {
        if (item->type() == BrainEditorItem::Type) {
            BrainEditorItem *eItem=(BrainEditorItem *)item;
            eItem->setPos(eItem->x()+EditorItem::_raster,eItem->y());
            eItem->setPos(eItem->x()-EditorItem::_raster,eItem->y());
        }
    }
}

void BrainEditor::addConnector(quint32 sourceId, quint32 destId)
{
    EditorItem *sourceItem=0;
    EditorItem *destItem=0;
    foreach (QGraphicsItem *item, items()) {
        if(item->type()==EditorItem::Type) {
            sourceItem=qgraphicsitem_cast<EditorItem*> (item);
            if(!sourceItem) {
                qCritical() << __PRETTY_FUNCTION__ << "No source item with id"  << sourceId;
                return;
            }
            if(sourceItem->getId()==sourceId) {
                break;
            }
        }
    }
    foreach (QGraphicsItem *item, items()) {
        if(item->type()==EditorItem::Type) {
            destItem=qgraphicsitem_cast<EditorItem*> (item);
            if(!destItem) {
                qCritical() << __PRETTY_FUNCTION__ << "No dest item with id"  << destId;
                return;
            }
            if(destItem->getId()==destId) {
                break;
            }
        }
    }
    EditorItemConnector *connector=new EditorItemConnector(sourceItem,destItem);
    FuzzyBase *fuzz=m_agentManager->getMasterAgent()->getBrain()->getFuzzy(destId);
    connector->setInverted(fuzz->isConnectionInverted(sourceId));
    addItem(connector);

    //! \bug  OK..Doing Here really nasty stuff. The Connectors are NOT SHOWING when scene is initially displayed So move all items one to the right and back again to schedule a redraw... Why the F*CK
    foreach (QGraphicsItem *item, items()) {
        if (item->type() == BrainEditorItem::Type) {
            BrainEditorItem *eItem=(BrainEditorItem *)item;
            eItem->setPos(eItem->x()+EditorItem::_raster,eItem->y());
            eItem->setPos(eItem->x()-EditorItem::_raster,eItem->y());
        }
    }
}

void BrainEditor::deleteFuzzyItem(quint32 id)
{
    foreach(QGraphicsItem *item,this->items()) {
        if(item->type()==EditorItem::Type) {
            EditorItem *eItem=(EditorItem *) item;
            if(eItem->getId()==id) {
                 eItem->removeConnectors();
            }
            removeItem(eItem);
            delete(eItem);
        }
    }
}

void BrainEditor::deleteConnector(quint32 sourceId, quint32 destId)
{
    foreach(QGraphicsItem *item,this->selectedItems()) {
        if(item->type()==EditorItemConnector::Type) {
            EditorItemConnector *conn=(EditorItemConnector*)item;
            if(conn->startItem()->getId()==sourceId && conn->endItem()->getId()==destId) {
                conn->startItem()->removeConnector(conn);
                conn->endItem()->removeConnector(conn);
                removeItem(conn);
                delete(conn);
                return;
            }
        }
    }
    //qCritical() << __PRETTY_FUNCTION__ << "Connector not found with source Id" << sourceId << "dest Id" << destId;
}

void BrainEditor::deleteSelectedItems()
{
    // First delete connections
    foreach(QGraphicsItem *item,this->selectedItems()) {
        if(item->type()==EditorItemConnector::Type) {
            EditorItemConnector *conn=(EditorItemConnector*)item;
            EditorItem *startItem=conn->startItem();
            EditorItem *endItem=conn->endItem();
            m_agentManager->deleteConnector(endItem->getId(),startItem->getId());
            conn->startItem()->removeConnector(conn);
            conn->endItem()->removeConnector(conn);
            removeItem(conn);
            delete(conn);
        }
    }
    foreach(QGraphicsItem *item,this->selectedItems()) {
        if(item->type()==EditorItem::Type) {
            EditorItem *eItem=(EditorItem *) item;
            // first delete connections to this item
            eItem->removeConnectors();
            m_agentManager->deleteFuzz(eItem->getId());
            removeItem(eItem);
            delete(eItem);
        }
    }
}

void BrainEditor::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    BrainEditorItem *item = qgraphicsitem_cast<BrainEditorItem *>(itemAt( event->scenePos().x(), event->scenePos().y(),QTransform()) );
    ItemEditorWidgetsBase::editMessage msg;
    if( item ) {
        BrainiacGlobals::ItemType iType=item->getType();
        if(     iType == BrainiacGlobals::OUTPUT ||
                iType == BrainiacGlobals::INPUT ||
                iType == BrainiacGlobals::NOISE ||
                iType == BrainiacGlobals::DEFUZZ ||
                iType == BrainiacGlobals::FUZZ
          ) {
            msg.object=item->getObject();
            msg.type=item->getType();
            msg.id=item->getId();
            emit itemClicked(msg);
        } else {
            qWarning() << __PRETTY_FUNCTION__ << "Item type not yet implemented";
        }
        if(m_altPressed) {
            if(m_connectSourceItem==0) {
                m_connectSourceItem=item;
                emit statusBarMessageChanged(QString("Select destination fuzz to connect"));
            } else {
                if( m_connectSourceItem!=item) {
                    //qDebug() << "Connect " << m_connectSourceItem->getId() << "with "<< item->getId();
                    EditorItemConnector *connector=new EditorItemConnector(m_connectSourceItem,item);
                    if(m_shiftPressed) {
                        m_agentManager->addConnector(item->getId(),m_connectSourceItem->getId(),true);
                        connector->setInverted(true);
                    } else {
                        m_agentManager->addConnector(item->getId(),m_connectSourceItem->getId(),false);
                    }
                    //connector->setInverted(fuzzChild->isConnectionInverted(fuzzy->getId()));
                    item->setPos(item->x()+EditorItem::_raster,item->y()); //! \bug move item to make it appear
                    item->setPos(item->x()-EditorItem::_raster,item->y());
                    addItem(connector);
                    emit statusBarMessageChanged(QString("Connected"));
                    m_connectSourceItem=0;
                } else {
                    emit statusBarMessageChanged(QString(""));
                    m_connectSourceItem=0;
                }
            }
        }
    }
}

void BrainEditor::updateItemLocations()
{
    foreach (QGraphicsItem *item, items()) {
        if (item->type() == BrainEditorItem::Type) {
            BrainEditorItem* bItem=(BrainEditorItem *) item;
            m_agentManager->setFuzzyEditorTranslation(bItem->getId(),(qint32)item->pos().x(),(qint32)item->pos().y());
        }
    }
}

void BrainEditor::keyPressEvent(QKeyEvent *event)
{

    if(event->key()==Qt::Key_Alt) {
      emit statusBarMessageChanged(QString("Select source fuzz to connect"));
      m_altPressed=true;
    }
    if(event->key()==Qt::Key_Shift) {
      m_shiftPressed=true;
    }
    if(event->key()==Qt::Key_Delete) {
        deleteSelectedItems();
    }
}

void BrainEditor::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Alt) {
      emit statusBarMessageChanged(QString(""));
      m_connectSourceItem=0;
      m_altPressed=false;
    }
    if(event->key()==Qt::Key_Shift) {
      m_shiftPressed=false;
    }
}

void BrainEditor::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QString dropAction=event->mimeData()->text();
    qCDebug(bGui) << __PRETTY_FUNCTION__ << "Droptext "<< dropAction;
    if(dropAction.startsWith(BrainiacGlobals::DropTextPrefix)) {
        BrainiacGlobals::ItemType type=(BrainiacGlobals::ItemType)dropAction.right(1).toInt();
        quint32 fuzzId=0;
        switch(type) {
        case BrainiacGlobals::AND:
            fuzzId=m_agentManager->addAndFuzz(event->scenePos().x(),event->scenePos().y());
            break;
        case BrainiacGlobals::DEFUZZ:
            fuzzId=m_agentManager->addDefuzz(event->scenePos().x(),event->scenePos().y());
            break;
        case BrainiacGlobals::FUZZ:
            fuzzId=m_agentManager->addFuzzFuzz(event->scenePos().x(),event->scenePos().y());
            break;
        case BrainiacGlobals::INPUT:
            fuzzId=m_agentManager->addInputFuzz(event->scenePos().x(),event->scenePos().y());
            break;
        case BrainiacGlobals::NOISE:
            fuzzId=m_agentManager->addNoiseFuzz(event->scenePos().x(),event->scenePos().y());
            break;
        case BrainiacGlobals::OR:
            fuzzId=m_agentManager->addOrFuzz(event->scenePos().x(),event->scenePos().y());
            break;
        case BrainiacGlobals::OUTPUT:
            fuzzId=m_agentManager->addOutputFuzz(event->scenePos().x(),event->scenePos().y());
            break;
        case BrainiacGlobals::TIMER:
            fuzzId=m_agentManager->addTimerFuzz(event->scenePos().x(),event->scenePos().y());
            break;
        default:
            qWarning()<< __PRETTY_FUNCTION__ << "Not a valid label!";
        }
    }
}
