#ifndef SCENEEDITOR_H
#define SCENEEDITOR_H

#include <QGraphicsScene>
#include "gui/itemeditorwidgetsbase.h"

class Scene;
class QGraphicsSceneDragDropEvent;

class SceneEditor : public QGraphicsScene
{
    Q_OBJECT
public:
    SceneEditor(Scene *scene);

protected:
    Scene *m_scene;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    void itemClicked(ItemEditorWidgetsBase::editMessage msg);
};

#endif // SCENEEDITOR_H
