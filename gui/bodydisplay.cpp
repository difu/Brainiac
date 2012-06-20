#include "bodydisplay.h"
#include "core/scene.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"

BodyDisplay::BodyDisplay(Scene *scene)
{
    m_scene=scene;
    m_rootSceneNode=0;
    m_agentManager=0;
    setWindowFlags(Qt::Tool);
    setGeometry(this->geometry().x(),this->geometry().y(),600,400);
    show();
}

void BodyDisplay::setAgentManager(AgentManager *manager)
{
    m_agentManager=manager;
    if(m_agentManager) {
        m_rootSceneNode=m_agentManager->getMasterAgent()->getBody()->getRootSkeletonNode();
        qDebug() << m_agentManager->getName() << "in BodyDisplay";
    }
}

void BodyDisplay::paintGL(QGLPainter *painter)
{
    painter->setStandardEffect(QGL::LitMaterial);
    //painter->setFaceColor(QGL::AllFaces, QColor(170, 202, 0));
    //m_rootSceneNode->draw(painter);
    if(m_rootSceneNode) {
        m_rootSceneNode->draw(painter);
    }
}
