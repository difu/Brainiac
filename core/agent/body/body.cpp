#include "body.h"
#include "core/agent/agent.h"
#include "core/agent/body/segment.h"

Body::Body(Agent *agent)
{
    m_agent=agent;
}

void Body::addSegment(Segment *segment)
{
    m_segments.append(segment);
}

Agent* Body::getAgent()
{
    return m_agent;
}

Segment * Body::getSegment(quint32 id)
{
    foreach(Segment *seg,m_segments) {
        if(seg->getId()==id)
            return seg;
    }
    return 0;
}

quint32 Body::getSegmentId(Segment *seg)
{
    foreach(Segment *segment,m_segments) {
        if(segment==seg)
            return seg->getId();
    }
    return 0;
}

QList<Segment *> Body::getSegments()
{
    return m_segments;
}
