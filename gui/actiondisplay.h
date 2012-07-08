#ifndef ACTIONDISPLAY_H
#define ACTIONDISPLAY_H
#include "gui/brainiacdisplay.h"
#include <QCloseEvent>

class Agent;
class Camera;
class Scene;
class QGLAbstractScene;
class SkeletonNode;
class QGLCamera;
class AgentManager;

/**
 * @brief The OpenGL view of the agent which agents are edited by the ActionEditor
 *
 * @sa ActionEditor
 * @class ActionDisplay actiondisplay.h "gui/actiondisplay.h"
 */
class ActionDisplay  : public BrainiacDisplay
{
    Q_OBJECT
public:
    ActionDisplay(QWidget *parent);
    /**
     * @brief sets the Agent to display
     *
     * @fn setAgent
     * @param agent the agent
     */
    void setAgent(Agent *agent);
    void setCameraOffset(const QVector3D &offset);

protected:
    void keyPressEvent(QKeyEvent *e);
    void paintGL(QGLPainter *painter);
    QVector3D m_cameraCenterPos;
    QVector3D m_cameraEyePos;

    Agent *m_agent;
    bool m_followAgent;
signals:
    void animationRunningToggled();
    void animationOneFrameForward();
    void animationOneFrameBackward();
};

#endif // ACTIONDISPLAY_H
