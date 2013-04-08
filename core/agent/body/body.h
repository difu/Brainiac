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
class Animation;
class BodySegment;

class Body
{
public:
    Body(Agent *agent);

    void addBodySegment( osg::ref_ptr<BodySegment> bodySegment, quint32 parentId);
    void copyBody(Body *body);
    QHash<quint32,Animation *> * getAnimations();
    AnimationPlayer* getAnimationPlayer() {return m_animationPlayer; }

    /**
     * @brief sets the current set of animations
     *
     * this allows setting of different animation sets of individual agents. Normally each agent instance of a group has a common set of animations. This allows setting of an individial set e.g. when editing in the action editor
     * @fn setAnimations
     * @param QHash<quint32
     * @param animations
     */
    void setAnimations(QHash<quint32,Animation *> *animations);

    osg::ref_ptr<osg::PositionAttitudeTransform> getRootSegment() { return m_rootSegment; }
    Agent* getAgent();
    void updatePosition();
    ~Body();
protected:
    Agent *m_agent;

    AnimationPlayer *m_animationPlayer;

    QHash<quint32, BodySegment* > m_bodySegments;

    osg::ref_ptr<osg::PositionAttitudeTransform> m_rootSegment;
};

#endif // BODY_H
