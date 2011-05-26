#ifndef EDITORBASE_H
#define EDITORBASE_H

#include <QGraphicsScene>

#include "gui/itemeditorwidgetsbase.h"

class Scene;


/** \brief  Base class for item based editors

        All item based editors (these which appear in center region of the main window) must subclass this

**/
class EditorBase : public QGraphicsScene
{
    Q_OBJECT
public:
    EditorBase(Scene *scene);

protected:
    Scene *m_scene;
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
signals:
    void itemClicked(ItemEditorWidgetsBase::editMessage msg);
};

#endif // EDITORBASE_H
