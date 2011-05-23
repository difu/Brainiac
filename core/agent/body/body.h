#ifndef BODY_H
#define BODY_H

#include <QList>

class Agent;
class Segment;

class Body
{
public:
    Body(Agent *agent);
    void addSegment(Segment *segment);
    Segment * getSegment( quint32 id);
    quint32 getSegmentId(Segment *seg);
    QList<Segment *> getSegments();
    Agent* getAgent();
protected:
    Agent *m_agent;
    QList<Segment *> m_segments;
};

#endif // BODY_H
