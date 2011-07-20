#ifndef BRAINEDITOR_H
#define BRAINEDITOR_H

#include <QGraphicsScene>
#include "gui/editorbase.h"

class AgentManager;
class Agent;

class BrainEditor : public EditorBase
{
    Q_OBJECT
public:
    explicit BrainEditor(Scene *scene, AgentManager *agentManager);
    Agent* getSelectedAgent();
    void setSelectedAgent(Agent *agent);
    void updateItemLocations();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    Agent *m_selectedAgent;
    AgentManager *m_agentManager;
signals:

public slots:

};

#endif // BRAINEDITOR_H
