#include "body.h"
#include "core/agent/agent.h"

Body::Body(Agent *agent)
{
    m_agent=agent;
}

Agent* Body::getAgent()
{
    return m_agent;
}
