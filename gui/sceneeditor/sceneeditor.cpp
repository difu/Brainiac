#include "sceneeditor.h"
#include "core/scene.h"
#include "core/group/group.h"
#include "core/brainiacglobals.h"
#include "gui/sceneeditor/sceneeditoritem.h"
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include "core/agent/agentmanager.h"

SceneEditor::SceneEditor(Scene *scene)
{
    m_scene=scene;
    setSceneRect(0, 0, 4000, 4000);
    foreach(Group *grp,m_scene->getGroups()) {
        qDebug() << "Group in editor added" << grp->getId();
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


void SceneEditor::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasText()) {
            event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void SceneEditor::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasText()) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
            event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void SceneEditor::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    SceneEditorItem *item = qgraphicsitem_cast<SceneEditorItem *>(itemAt( event->scenePos().x(), event->scenePos().y()) );
    ItemEditorWidgetsBase::editMessage msg;
    if( item ) {
        if(item->getType() == BrainiacGlobals::GROUP) {
            msg.object=item->getObject();
            msg.type=BrainiacGlobals::GROUP;
            emit itemClicked(msg);
        }
    }
}
