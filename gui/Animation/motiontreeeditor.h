#ifndef MOTIONTREEEDITOR_H
#define MOTIONTREEEDITOR_H

#include <QGraphicsScene>
#include "gui/editorbase.h"
#include "core/brainiacglobals.h"

class MotionTree;
class AgentManager;

class MotionTreeEditor : public EditorBase
{
    Q_OBJECT
public:
    explicit MotionTreeEditor(MotionTree *motionTree, Scene *scene);

protected:
    MotionTree *m_motionTree;

signals:

public slots:

};

#endif // MOTIONTREEEDITOR_H
