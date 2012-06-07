#ifndef BODY_H
#define BODY_H

#include <QList>

class Agent;
class Segment;

class SkeletonNode;

class Body
{
public:
    Body(Agent *agent, Body *body=0);
    void addSegment(Segment *segment);
    void addSkeletonNode( SkeletonNode *node, quint32 parentId );
    SkeletonNode *getRootSkeletonNode() { return m_rootSegment; }
    Segment * getSegment( quint32 id);
    quint32 getSegmentId(Segment *seg);
    QList<Segment *> getSegments();
    Agent* getAgent();
    void renderGL();
    void updatePosition();
    ~Body();
protected:
    void copySkeletonNode(SkeletonNode *n);
    Agent *m_agent;
    QList<Segment *> m_segments;

    SkeletonNode *m_rootSegment;
};

#endif // BODY_H
