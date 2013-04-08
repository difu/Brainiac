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


#include "body.h"
#include "core/scene.h"
#include "core/simulation.h"
#include "core/agent/agent.h"
#include "core/agent/body/animation/animationplayer.h"
#include "core/agent/channel.h"
#include "core/agent/body/bodysegment.h"

#include <QColor>
#include <glu.h>
#include <GLUT/glut.h>


Body::Body(Agent *agent)
{
    m_agent=agent;
    m_animationPlayer=new AnimationPlayer(this);
    m_rootSegment=new osg::PositionAttitudeTransform;
    m_rootSegment.get()->setName("AgentBody Root Segment");

}

void Body::addBodySegment(osg::ref_ptr<BodySegment> bodySegment, quint32 parentId)
{
    //qDebug( ) << __PRETTY_FUNCTION__ << "Added BodySegment with parentId" << parentId;
    //BodySegment *bs=bodySegment.get();
    m_bodySegments.insert(bodySegment.get()->getId(),bodySegment.get());
    if(parentId==0) {
        osg::PositionAttitudeTransform *trans=m_rootSegment.get();
        trans->addChild(bodySegment);
        m_rootSegment.get()->addChild(bodySegment.get());
        return;
    }
    BodySegment *bs=m_bodySegments.value(parentId,0);
    if(bs) {
        bs->addChild(bodySegment);
    } else {
        qCritical() << __PRETTY_FUNCTION__ << "Unknown parentId " << parentId;
    }
}

Agent* Body::getAgent()
{
    return m_agent;
}

QHash<quint32,Animation *> * Body::getAnimations()
{
    return m_animationPlayer->getAnimations();
}

void Body::setAnimations(QHash<quint32, Animation *> *animations)
{
    m_animationPlayer->setAnimations(animations);
}

void Body::updatePosition() {
    if(m_agent) {
       osg::PositionAttitudeTransform *posTrans=m_rootSegment.get();
        posTrans->setPosition(osg::Vec3d(m_agent->getPosition()->x(),m_agent->getPosition()->y(),m_agent->getPosition()->z()));
    }
}

Body::~Body()
{
    m_agent->getScene()->getRootSceneNode()->removeChild(m_rootSegment);
}
