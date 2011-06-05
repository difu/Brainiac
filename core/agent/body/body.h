#ifndef BODY_H
#define BODY_H

#include <QList>

class Agent;
class Segment;

class Body
{
public:
    Body(Agent *agent, Body *body=0);
    void addSegment(Segment *segment);
    Segment * getSegment( quint32 id);
    quint32 getSegmentId(Segment *seg);
    QList<Segment *> getSegments();
    Agent* getAgent();
    void renderGL();
    ~Body();
protected:
    Agent *m_agent;
    QList<Segment *> m_segments;
};

#endif // BODY_H
