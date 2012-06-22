#include "bodydisplay.h"
#include "core/scene.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"

#include <QKeyEvent>

BodyDisplay::BodyDisplay(Scene *scene)
{
    m_scene=scene;
    m_rootSceneNode=0;
    m_agentManager=0;
    m_renderSilhouettes=false;
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

void BodyDisplay::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_S) {
        if(m_agentManager) {
            m_renderSilhouettes=!m_renderSilhouettes;
            m_agentManager->getMasterAgent()->getBody()->renderSilhouettes(m_renderSilhouettes);
            updateGL();
        }
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
