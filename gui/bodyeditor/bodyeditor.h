#ifndef BODYEDITOR_H
#define BODYEDITOR_H
#include <QGraphicsScene>
#include "gui/editorbase.h"

class AgentManager;
class Agent;

class BodyEditor : public EditorBase
{
    Q_OBJECT
public:
    explicit BodyEditor(Scene *scene, AgentManager *agentManager);
    
protected:
    AgentManager *m_agentManager; //!< the agentmanager
signals:
    
public slots:
    
};

#endif // BODYEDITOR_H
