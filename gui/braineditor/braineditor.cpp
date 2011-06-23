#include "braineditor.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/output.h"
#include "gui/braineditor/braineditoritem.h"
#include "gui/editoritemconnector.h"
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

BrainEditor::BrainEditor(Scene *scene, AgentManager *agentManager) : EditorBase(scene)
{
    m_agentManager=agentManager;
    m_selectedAgent=0;
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
        }  else if(fuzzy->getType()==FuzzyBase::NOISE) {
            BrainEditorItem *item=new BrainEditorItem(BrainiacGlobals::NOISE,m_agentManager,fuzzy->getId());
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
            eItem->setPos(eItem->x()+1,eItem->y());
            eItem->setPos(eItem->x()-1,eItem->y());
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
        }
    }
}
