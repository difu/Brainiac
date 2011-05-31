#include "braineditor.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/output.h"
#include "gui/braineditor/braineditoritem.h"
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

BrainEditor::BrainEditor(Scene *scene, AgentManager *agentManager) : EditorBase(scene)
{
    m_agentManager=agentManager;
    foreach(FuzzyBase *fuzzy, m_agentManager->getMasterAgent()->getBrain()->getFuzzies())
    {
        if(fuzzy->getType()==FuzzyBase::OUTPUT) {
            qDebug() << "Placing output";
            BrainEditorItem *item=new BrainEditorItem(BrainiacGlobals::OUTPUT,m_agentManager,fuzzy->getId());
            item->setPos(m_agentManager->getMasterAgent()->getBrain()->getFuzzy(fuzzy->getId())->getEditorTranslationX(),m_agentManager->getMasterAgent()->getBrain()->getFuzzy(fuzzy->getId())->getEditorTranslationY());
            //item->setFlag();
            addItem(item);
        }
    }
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
        }
    }
}
