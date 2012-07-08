#ifndef BODYDISPLAY_H
#define BODYDISPLAY_H

#include "gui/brainiacdisplay.h"
#include <QCloseEvent>

class Camera;
class Scene;
class QGLAbstractScene;
class SkeletonNode;
class QGLCamera;
class AgentManager;

class BodyDisplay : public BrainiacDisplay
{
    Q_OBJECT
public:
    BodyDisplay(Scene *scene);
    void setAgentManager( AgentManager *manager );

protected:
    void keyPressEvent(QKeyEvent *e);
    void paintGL(QGLPainter *painter);

    SkeletonNode *m_rootSkeletonNode;
    Scene *m_scene;
    AgentManager *m_agentManager;
};

#endif // BODYDISPLAY_H
