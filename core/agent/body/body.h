// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#ifndef BODY_H
#define BODY_H

#include <QList>
#include <QHash>

#include <osg/PositionAttitudeTransform>

class Agent;
class AnimationPlayer;
class SkeletonNode;
class Animation;
class QGLPainter;
class BodySegment;

class Body
{
public:
    Body(Agent *agent, Body *body=0);

    void addBodySegment( osg::ref_ptr<BodySegment> bodySegment, quint32 parentId);
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

    osg::ref_ptr<osg::PositionAttitudeTransform> getRootSegment() { return m_rootSegment; }

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

    QHash<quint32, BodySegment* > m_bodySegments;

    osg::ref_ptr<osg::PositionAttitudeTransform> m_rootSegment;
};

#endif // BODY_H
