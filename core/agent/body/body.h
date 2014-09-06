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
#include <osg/Switch>

class Agent;
class AnimationPlayer;
class Animation;
class BodySegment;
class MotionTreeAnimationPlayer;

/**
 * @brief The Agents Body
 *
 * Manages the Body of an Agent
 */
class Body
{
public:
    Body(Agent *agent);

    void addAnimation(Animation *animation);

    void addBodySegment( osg::ref_ptr<BodySegment> bodySegment, quint32 parentId);

    /**
     * @brief advances the body
     * the matrice(s) of the segments will be calculated if necessary
     */
    void advance();

    void applyPlayers();
    QHash<QString, Animation *> *getAnimations();
    BodySegment* getBodySegment(quint32 id) { return m_bodySegments.value(id,0); }
    AnimationPlayer* getAnimationPlayer() {return m_animationPlayer; }
    QList<MotionTreeAnimationPlayer *>  getMotionTreeAnimationPlayers() { return m_treeAnimationPlayers; }

    void highlightSegment(quint32 id, bool unselectOthers=true);

    void showPositionMarker(bool show);

    void showPivotCoordCrosses(bool show);

    void reset();

    void toggleShowCoordCrosses();

    /**
     * @brief toggles the position marker
     *
     * the position marker shows the actual position and orientation of an agent
     */
    void toggleShowPositionMarker();

    /**
     * @brief toggles the dispaly of the skeleton
     *
     */
    void toggleShowSkeleton();

    osg::ref_ptr<osg::PositionAttitudeTransform> getBodyRoot() { return m_bodyRoot; }
    Agent* getAgent();
    void updatePosition();
    ~Body();
protected:
    Agent *m_agent;

    AnimationPlayer *m_animationPlayer;
    QList<MotionTreeAnimationPlayer *> m_treeAnimationPlayers;

    QHash<quint32, BodySegment* > m_bodySegments;

    osg::ref_ptr<osg::PositionAttitudeTransform> m_bodyRoot;
    osg::ref_ptr<osg::Switch> m_switchPositionMarker; /**< Switch to toggle coord cross at agents position */
    osg::ref_ptr<osg::Switch> m_switchSkeleton; /**< Switch to toggle coord cross at agents position */
    bool m_showPositionMarker; /**< property to toggle position marker */
    bool m_showAllCoordCrosses; /**< property to toggle to show all coord crosses of all segments */
    bool m_showSkeleton; /**< property to toggle to show the skeleton */
};

#endif // BODY_H
