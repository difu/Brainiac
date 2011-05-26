#ifndef BRAINEDITOR_H
#define BRAINEDITOR_H

#include <QGraphicsScene>
#include "gui/editorbase.h"

class AgentManager;

class BrainEditor : public EditorBase
{
    Q_OBJECT
public:
    explicit BrainEditor(Scene *scene, AgentManager *agentManager);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    AgentManager *m_agentManager;
signals:

public slots:

};

#endif // BRAINEDITOR_H
