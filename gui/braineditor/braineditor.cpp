#include "braineditor.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/output.h"
#include "gui/braineditor/braineditoritem.h"
#include "gui/editoritemconnector.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>

BrainEditor::BrainEditor(Scene *scene, AgentManager *agentManager) : EditorBase(scene)
{
    m_agentManager=agentManager;
    m_selectedAgent=0;
    m_altPressed=false;
    m_shiftPressed=false;
    m_connectSourceItem=0;
    foreach(FuzzyBase *fuzzy, m_agentManager->getMasterAgent()->getBrain()->getFuzzies())
    {
        if(fuzzy->getType()==FuzzyBase::OUTPUT) {
            BrainEditorItem *item=new BrainEditorItem(BrainiacGlobals::OUTPUT,m_agentManager,fuzzy->getId());
            item->setPos(m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).x(),m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).y());
            //item->setFlag();
            addItem(item);
            continue;
        } else if(fuzzy->getType()==FuzzyBase::INPUT) {
            BrainEditorItem *item=new BrainEditorItem(BrainiacGlobals::INPUT,m_agentManager,fuzzy->getId());
            item->setPos(m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).x(),m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).y());
            //item->setFlag();
            addItem(item);
            continue;
        } else if(fuzzy->getType()==FuzzyBase::NOISE) {
            BrainEditorItem *item=new BrainEditorItem(BrainiacGlobals::NOISE,m_agentManager,fuzzy->getId());
            item->setPos(m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).x(),m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).y());
            addItem(item);
            continue;
        } else if(fuzzy->getType()==FuzzyBase::AND) {
            BrainEditorItem *item=new BrainEditorItem(BrainiacGlobals::AND,m_agentManager,fuzzy->getId());
            item->setPos(m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).x(),m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).y());
            addItem(item);
            continue;
        } else if(fuzzy->getType()==FuzzyBase::OR) {
            BrainEditorItem *item=new BrainEditorItem(BrainiacGlobals::OR,m_agentManager,fuzzy->getId());
            item->setPos(m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).x(),m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).y());
            addItem(item);
            continue;
        } else if(fuzzy->getType()==FuzzyBase::DEFUZZ) {
            BrainEditorItem *item=new BrainEditorItem(BrainiacGlobals::DEFUZZ,m_agentManager,fuzzy->getId());
            item->setPos(m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).x(),m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).y());
            addItem(item);
            continue;
        } else if(fuzzy->getType()==FuzzyBase::FUZZ) {
            BrainEditorItem *item=new BrainEditorItem(BrainiacGlobals::FUZZ,m_agentManager,fuzzy->getId());
            item->setPos(m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).x(),m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).y());
            addItem(item);
            continue;
        } else if(fuzzy->getType()==FuzzyBase::TIMER) {
            BrainEditorItem *item=new BrainEditorItem(BrainiacGlobals::TIMER,m_agentManager,fuzzy->getId());
            item->setPos(m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).x(),m_agentManager->getEditorFuzzyLocations().value(fuzzy->getId()).y());
            addItem(item);
            continue;
        }
    }
    foreach(FuzzyBase *fuzzy, m_agentManager->getMasterAgent()->getBrain()->getFuzzies())
    {
        if(fuzzy->hasChildren()) {
            EditorItem *eItemParent=0;
            EditorItem *eItemChild=0;
            foreach (QGraphicsItem *item, items()) {
                if(item->type()==EditorItem::Type) {
                    eItemParent=qgraphicsitem_cast<EditorItem*> (item);
                    if(eItemParent) {
                        if(eItemParent->getId()==fuzzy->getId()) {
                            //qDebug( ) << "Found parent" << eItemParent->getId();
                            foreach(FuzzyBase *fuzzChild, fuzzy->getChildren()) {
                                foreach (QGraphicsItem *childItem, items()) {
                                    if(childItem->type()==EditorItem::Type) {
                                        eItemChild=qgraphicsitem_cast<EditorItem*> (childItem);
                                        if(eItemChild->getId()==fuzzChild->getId()) {
                                            EditorItemConnector *connector=new EditorItemConnector(eItemParent,eItemChild);
                                            connector->setInverted(fuzzChild->isConnectionInverted(fuzzy->getId()));
                                            addItem(connector);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
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

void BrainEditor::deleteSelectedItems()
{
    // First delete connections
    foreach(QGraphicsItem *item,this->selectedItems()) {
        if(item->type()==EditorItemConnector::Type) {
            qDebug() << "Found connector 2b deleted";
            EditorItemConnector *conn=(EditorItemConnector*)item;
            EditorItem *startItem=conn->startItem();
            EditorItem *endItem=conn->endItem();
            m_agentManager->deleteConnector(endItem->getId(),startItem->getId());
            removeItem(conn);
            delete(conn);
        }
    }
    foreach(QGraphicsItem *item,this->selectedItems()) {
        if(item->type()==EditorItem::Type) {
            qDebug() << "Found item 2b deleted";
        }
    }
}

Agent* BrainEditor::getSelectedAgent()
{
    return m_selectedAgent;
}

void BrainEditor::setSelectedAgent(Agent *agent)
{
    m_selectedAgent=agent;
}

void BrainEditor::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    BrainEditorItem *item = qgraphicsitem_cast<BrainEditorItem *>(itemAt( event->scenePos().x(), event->scenePos().y()) );
    ItemEditorWidgetsBase::editMessage msg;
    if( item ) {
        if(item->getType() == BrainiacGlobals::OUTPUT) {
            msg.object=item->getObject();
            msg.type=BrainiacGlobals::OUTPUT;
            msg.id=item->getId();
            emit itemClicked(msg);
        } else if(item->getType() == BrainiacGlobals::INPUT) {
            msg.object=item->getObject();
            msg.type=BrainiacGlobals::INPUT;
            msg.id=item->getId();
            emit itemClicked(msg);
        } else if(item->getType() == BrainiacGlobals::NOISE) {
            msg.object=item->getObject();
            msg.type=BrainiacGlobals::NOISE;
            msg.id=item->getId();
            emit itemClicked(msg);
        } else if(item->getType() == BrainiacGlobals::DEFUZZ) {
            msg.object=item->getObject();
            msg.type=BrainiacGlobals::DEFUZZ;
            msg.id=item->getId();
            emit itemClicked(msg);
        } else if(item->getType() == BrainiacGlobals::FUZZ) {
            msg.object=item->getObject();
            msg.type=BrainiacGlobals::FUZZ;
            msg.id=item->getId();
            emit itemClicked(msg);
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
    qDebug() << "Droptext "<< dropAction;
    if(dropAction.startsWith("AddLogicType")) {
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
            qDebug()<<__PRETTY_FUNCTION__ << "Not a valid label!";
        }
        if(fuzzId>0) {
            BrainEditorItem *item=new BrainEditorItem(type,m_agentManager,fuzzId);
            item->setPos(event->scenePos().x()+BrainEditorItem::_width,event->scenePos().y());
            addItem(item);
        }

    }

}
