#ifndef ACTIONEDITOR_H
#define ACTIONEDITOR_H

#include <QDialog>

class Agent;
class AgentManager;
class Animation;
class SkeletonNode;
class ActionDisplay;
class Scene;

namespace Ui {
class ActionEditor;
}

/**
 * @brief the ActionEditor to edit the actions (Animation) of an Agent
 *
 * @class ActionEditor actioneditor.h "gui/Animation/actioneditor.h"
 */
class ActionEditor : public QDialog
{
    Q_OBJECT
    
public:
    explicit ActionEditor(Scene *scene, QWidget *parent = 0);
    void setAgentManager(AgentManager *manager);
    ~ActionEditor();

public slots:
    void hide();
    void show();
protected:
    void setActiveAnimation(quint32 animId);
    void addCurvesToList(SkeletonNode *node, quint32 level);
    void timerEvent(QTimerEvent *);
    AgentManager *m_agentManager;
    Agent *m_agent;
    Animation *m_activeAnimation;
    ActionDisplay *m_actionDisplay; /**< TODO */
    Scene *m_scene;
protected slots:
    void animationSelectionChanged(int rowId);
private:
    Ui::ActionEditor *ui;
};

#endif // ACTIONEDITOR_H
