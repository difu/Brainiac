#ifndef BODY_H
#define BODY_H

#include <QList>

class Agent;

class SkeletonNode;

class Body
{
public:
    Body(Agent *agent, Body *body=0);
    void addSkeletonNode( SkeletonNode *node, quint32 parentId );
    void copyBody(Body *body);
    /**
     * @brief returns a list of all SkeletonNodes
     * Returns only SkeletonNodes and NO geometry nodes. The root SkeletonNode is also NOT included!
     * @sa getRootSkeletonNode()
     * @return QList<SkeletonNode *>
     */
    QList<SkeletonNode *> getAllSkeletonNodes();
    SkeletonNode *getRootSkeletonNode() { return m_rootSkeletonNode; }
    SkeletonNode *getSkeletonNodeById(quint32 id);
    Agent* getAgent();
    void renderSilhouettes(bool render);
    void updatePosition();
    ~Body();
protected:
    void copySkeletonNode(SkeletonNode *n);
    Agent *m_agent;

    SkeletonNode *m_rootSkeletonNode;
};

#endif // BODY_H
