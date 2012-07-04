#include "bodydisplay.h"
#include "core/scene.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"

#include <QKeyEvent>

BodyDisplay::BodyDisplay(Scene *scene): BrainiacDisplay(0)
{
    m_scene=scene;
    m_rootSkeletonNode=0;
    m_agentManager=0;
    m_renderSilhouettes=false;
    m_renderSkeleton=false;
//    setWindowFlags(Qt::Tool);
//    setGeometry(this->geometry().x(),this->geometry().y(),600,400);
   show();
}

void BodyDisplay::setAgentManager(AgentManager *manager)
{
    m_agentManager=manager;
    if(m_agentManager) {
        m_rootSkeletonNode=m_agentManager->getMasterAgent()->getBody()->getRootSkeletonNode();
        qDebug() << m_agentManager->getName() << "in BodyDisplay";
    }
}

void BodyDisplay::keyPressEvent(QKeyEvent *e)
{
    BrainiacDisplay::keyPressEvent(e);
    if(e->key()==Qt::Key_S) {
        if(!m_shiftPressed) {
            if(m_agentManager) {
                m_renderSilhouettes=!m_renderSilhouettes;
                m_agentManager->getMasterAgent()->getBody()->showSilhouettes(m_renderSilhouettes);
                updateGL();
            }
        } else {
            m_renderSkeleton=!m_renderSkeleton;
            updateGL();
        }
    }
}

void BodyDisplay::paintGL(QGLPainter *painter)
{
    painter->setStandardEffect(QGL::LitMaterial);
    //painter->setFaceColor(QGL::AllFaces, QColor(170, 202, 0));
    //m_rootSceneNode->draw(painter);
    if(m_rootSkeletonNode) {
        m_rootSkeletonNode->draw(painter);
        if(m_renderSkeleton) {
            m_agentManager->getMasterAgent()->getBody()->renderSkeleton(painter);
        }
    }
}

