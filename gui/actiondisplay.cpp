#include "actiondisplay.h"

#include "core/scene.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"

ActionDisplay::ActionDisplay(QWidget *parent): BrainiacDisplay(parent)
{
    m_agent=0;
    m_followAgent=false;
    m_renderSilhouettes=false;
    m_renderSkeleton=false;
    hide();
}

void ActionDisplay::keyPressEvent(QKeyEvent *e)
{
    BrainiacDisplay::keyPressEvent(e);
    if(e->key()==Qt::Key_F) {
        m_followAgent=!m_followAgent;
    } else if(e->key()==Qt::Key_Space) {
        emit animationRunningToggled();
    } else if(e->key()==Qt::Key_Left) {
        emit animationOneFrameBackward();
    } else if(e->key()==Qt::Key_Right) {
        emit animationOneFrameForward();
    }
}

void ActionDisplay::paintGL(QGLPainter *painter)
{
    BrainiacDisplay::paintGL(painter);
    painter->setStandardEffect(QGL::LitMaterial);

    if(m_agent)
    {
        m_agent->getBody()->renderSkeleton(painter);
        m_agent->getBody()->getRootSkeletonNode()->draw(painter);
    }
    if(!m_followAgent) {
        m_cameraCenterPos=m_camera->center();
        m_cameraEyePos=m_cameraEyePos;
    }
}

void ActionDisplay::setAgent(Agent *agent)
{
    m_agent=agent;
}

void ActionDisplay::setCameraOffset(const QVector3D &offset)
{
    if(m_followAgent) {
        QVector3D v=m_camera->eye()-m_camera->center();
        m_camera->setCenter(offset+m_cameraCenterPos);
        m_camera->setEye(m_camera->center()+v);
        m_camera->setUpVector(QVector3D(0,1,0));
    }
}

