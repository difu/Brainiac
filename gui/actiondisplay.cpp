#include "actiondisplay.h"

#include "core/scene.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"

ActionDisplay::ActionDisplay(QWidget *parent): BrainiacDisplay(parent)
{
    m_agent=0;
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
    BrainiacDisplay::paintGL(painter);
    painter->setStandardEffect(QGL::LitMaterial);

    if(m_agent)
    {
        m_agent->getBody()->renderSkeleton(painter);
        m_agent->getBody()->getRootSkeletonNode()->draw(painter);
    }
}

void ActionDisplay::setAgent(Agent *agent)
{
    m_agent=agent;
}

