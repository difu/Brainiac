#ifndef BODYDISPLAY_H
#define BODYDISPLAY_H

#include <qglview.h>
#include <QCloseEvent>

class Camera;
class Scene;
class QGLAbstractScene;
class QGLSceneNode;
class QGLCamera;
class AgentManager;

class BodyDisplay : public QGLView
{
    Q_OBJECT
public:
    BodyDisplay(Scene *scene);
    void setAgentManager( AgentManager *manager );

protected:
    void keyPressEvent(QKeyEvent *e);
    void paintGL(QGLPainter *painter);
    QGLSceneNode *m_rootSceneNode;
    Scene *m_scene;
    AgentManager *m_agentManager;
    bool m_renderSilhouettes;
};

#endif // BODYDISPLAY_H
