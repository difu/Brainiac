#ifndef ACTIONDISPLAY_H
#define ACTIONDISPLAY_H
#include "gui/brainiacdisplay.h"
#include <QCloseEvent>

class Camera;
class Scene;
class QGLAbstractScene;
class SkeletonNode;
class QGLCamera;
class AgentManager;

/**
 * @brief
 *
 * @class ActionDisplay actiondisplay.h "gui/actiondisplay.h"
 * @bug paintGL() m_rootSkeletonNode->draw(painter); does not work....
 */
class ActionDisplay  : public BrainiacDisplay
{
    Q_OBJECT
public:
    ActionDisplay(QWidget *parent);
    void setAgentManager( AgentManager *manager );

protected:
    void keyPressEvent(QKeyEvent *e);
    void paintGL(QGLPainter *painter);

    SkeletonNode *m_rootSkeletonNode;
    Scene *m_scene;
    AgentManager *m_agentManager;
    bool m_renderSilhouettes;
    bool m_renderSkeleton;
};

#endif // ACTIONDISPLAY_H
