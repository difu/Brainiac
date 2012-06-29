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

    /**
     * @brief Renders the skeleton
     *
     * Renders recursivly the skeleton of an agent
     * @fn renderSkeleton
     * @param painter the QGLPainter to paint on
     * @param node the root node of an agent
     */
    void renderSkeleton(QGLPainter *painter, SkeletonNode *node);
    SkeletonNode *m_rootSkeletonNode;
    Scene *m_scene;
    AgentManager *m_agentManager;
    bool m_renderSilhouettes;
    bool m_renderSkeleton;
};

#endif // BODYDISPLAY_H
