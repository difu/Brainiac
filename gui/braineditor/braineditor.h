#ifndef BRAINEDITOR_H
#define BRAINEDITOR_H

#include <QGraphicsScene>
#include "gui/editorbase.h"

class AgentManager;
class Agent;
class BrainEditorItem;

class BrainEditor : public EditorBase
{
    Q_OBJECT
public:
    explicit BrainEditor(Scene *scene, AgentManager *agentManager);
    Agent* getSelectedAgent();
    void setSelectedAgent(Agent *agent);
    void updateItemLocations();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    bool m_altPressed; //!< true, if alt key is pressed
    bool m_shiftPressed; //!< true, if shift key is pressed
    BrainEditorItem *m_connectSourceItem; //!< holds the source fuzz when in connection mode
    Agent *m_selectedAgent; //!< the currently selected agent of this editor. This agent´s fuzz results are displayed
    AgentManager *m_agentManager; //!< the agentmanager of this braineditor
signals:
    void statusBarMessageChanged( const QString & message );
public slots:

};

#endif // BRAINEDITOR_H
