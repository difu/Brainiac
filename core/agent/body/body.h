#ifndef BODY_H
#define BODY_H

#include <QList>
#include <QHash>

class Agent;
class AnimationPlayer;
class SkeletonNode;
class Animation;
class QGLPainter;

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
    QHash<quint32,Animation *> * getAnimations();
    AnimationPlayer* getAnimationPlayer() {return m_animationPlayer; }

    /**
     * @brief paints the skeleton on the QGLPainter
     *
     * @fn renderSkeleton
     * @param painter the painter to paint on
     */
    void renderSkeleton(QGLPainter *painter);

    /**
     * @brief sets the current set of animations
     *
     * this allows setting of different animation sets of individual agents. Normally each agent instance of a group has a common set of animations. This allows setting of an individial set e.g. when editing in the action editor
     * @fn setAnimations
     * @param QHash<quint32
     * @param animations
     */
    void setAnimations(QHash<quint32,Animation *> *animations);

    /**
     * @brief returns the root SkeletonNode
     *
     * the root SkeletonNode is NOT the first bone! It is more like a virtual point to which the rest refers
     * @fn getRootSkeletonNode
     * @return SkeletonNode
     */
    SkeletonNode *getRootSkeletonNode() { return m_rootSkeletonNode; }

    /**
     * @brief returns the root bone
     *
     * the root bone is the first bone node of the skeleton. (e.g. the ROOT element of a bvh file)
     * @fn getRootBone
     * @return SkeletonNode
     */
    SkeletonNode *getRootBone();
    SkeletonNode *getSkeletonNodeById(quint32 id);
    SkeletonNode *getSkeletonNodeByName(const QString &name);
    Agent* getAgent();
    void showBoneCoordCrosses(bool show);
    void showSilhouettes(bool show);
    void updatePosition();
    ~Body();
protected:
    void copySkeletonNode(SkeletonNode *n);

    /**
     * @brief Renders the skeleton
     *
     * Renders recursivly the skeleton of an agent
     * @fn renderSkeleton
     * @param painter the QGLPainter to paint on
     * @param node the root node of an agent
     */
    void renderSkeleton(QGLPainter *painter, SkeletonNode *node) const;
    Agent *m_agent;

    SkeletonNode *m_rootSkeletonNode;
    AnimationPlayer *m_animationPlayer;
};

#endif // BODY_H
