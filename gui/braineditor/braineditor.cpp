#include "braineditor.h"
#include "core/agent/agentmanager.h"

BrainEditor::BrainEditor(Scene *scene, AgentManager *agentManager) : EditorBase(scene)
{
    m_agentManager=agentManager;
}


void BrainEditor::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
//    SceneEditorItem *item = qgraphicsitem_cast<SceneEditorItem *>(itemAt( event->scenePos().x(), event->scenePos().y()) );
//    ItemEditorWidgetsBase::editMessage msg;
//    if( item ) {
//        if(item->getType() == BrainiacGlobals::GROUP) {
//            msg.object=item->getObject();
//            msg.type=BrainiacGlobals::GROUP;
//            emit itemClicked(msg);
//        }
//    }
}
