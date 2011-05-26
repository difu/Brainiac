#ifndef SCENEEDITOR_H
#define SCENEEDITOR_H

#include <QGraphicsScene>
#include "gui/editorbase.h"


class QGraphicsSceneDragDropEvent;

class SceneEditor : public EditorBase
{
    Q_OBJECT
public:
    SceneEditor(Scene *scene);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

};

#endif // SCENEEDITOR_H
