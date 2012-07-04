#include "actiondisplay.h"

#include "core/scene.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"

ActionDisplay::ActionDisplay(QWidget *parent): BrainiacDisplay(parent)
{
    m_rootSkeletonNode=0;
    m_agentManager=0;
    m_renderSilhouettes=false;
    m_renderSkeleton=false;
    hide();
}

void ActionDisplay::keyPressEvent(QKeyEvent *e)
{
    BrainiacDisplay::keyPressEvent(e);
}

void ActionDisplay::paintGL(QGLPainter *painter)
{
    painter->setStandardEffect(QGL::LitMaterial);
    if(m_agentManager) {
        m_agentManager->getMasterAgent()->getBody()->renderSkeleton(painter);
    }
//    if(m_rootSkeletonNode) {
//        m_rootSkeletonNode->draw(painter);
//    }
}

void ActionDisplay::setAgentManager(AgentManager *manager)
{
    m_agentManager=manager;
    m_rootSkeletonNode=m_agentManager->getMasterAgent()->getBody()->getRootSkeletonNode();
}
