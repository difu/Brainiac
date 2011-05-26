#include "editorbase.h"
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>


EditorBase::EditorBase(Scene *scene)
{
    m_scene=scene;
    setSceneRect(0, 0, 4000, 4000);
}


void EditorBase::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasText()) {
            event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void EditorBase::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasText()) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
            event->acceptProposedAction();
    } else {
        event->ignore();
    }
}
