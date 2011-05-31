#ifndef SCENEEDITOR_H
#define SCENEEDITOR_H

#include <QGraphicsScene>
#include "gui/editorbase.h"


class QGraphicsSceneDragDropEvent;

/** \brief  The graphical editor for this scene´s elements

        This editor allows easy selecting of the main contents of this scene
        The selected group also is the agent instance to be edited in braineditor and bodyeditor
        @sa BrainEditor

**/
class SceneEditor : public EditorBase
{
    Q_OBJECT
public:
    SceneEditor(Scene *scene);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

};

#endif // SCENEEDITOR_H
