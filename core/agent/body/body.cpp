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
#include "core/agent/agentmanager.h"
#include "core/agent/body/animation/animation.h"
#include "core/agent/body/animation/animationplayer.h"
#include "core/agent/body/animation/motiontreemanager.h"
#include "core/agent/body/animation/motiontree.h"
#include "core/agent/body/animation/motiontreeanimationplayer.h"
#include "core/agent/channel.h"
#include "core/agent/body/bodysegment.h"

#include <QColor>



Body::Body(Agent *agent)
{
    m_agent=agent;
    m_animationPlayer=new AnimationPlayer(this);
    for(quint32 i=0; i<MotionTreeManager::NUM_OF_TREE_TRACKS;i++) {
        MotionTreeAnimationPlayer *player=new MotionTreeAnimationPlayer(this,m_agent->getAgentManager()->getMotionTreeManager()->getMotionTrees().value(i));
        m_treeAnimationPlayers.append(player);
    }
    foreach(QString triggerName, m_agent->getAgentManager()->getMotionTreeManager()->getTriggers()) {
        Channel *tc=new Channel(0.0,BrainiacGlobals::ActionForceTriggerValue);
        m_agent->addOutputChannel(tc,triggerName);
    }
    m_bodyRoot=new osg::PositionAttitudeTransform;
    m_bodyRoot.get()->setName("AgentBody Root Segment");
    m_showAllCoordCrosses=false;
    m_showSkeleton=false;
    m_switchPositionMarker=new osg::Switch;
    osg::Geode *geo=new osg::Geode;
    geo->addDrawable(BrainiacGlobals::CoordCross.get());

    m_switchSkeleton=new osg::Switch;
    m_bodyRoot->addChild(m_switchSkeleton);

    // Switch Lightning off for position marker
    osg::StateSet *state = geo->getOrCreateStateSet();
    state->setMode( GL_LIGHTING, osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF );

    m_switchPositionMarker->addChild(geo,false);
    m_bodyRoot->addChild(m_switchPositionMarker);
    m_showPositionMarker=false;
}

void Body::addBodySegment(osg::ref_ptr<BodySegment> bodySegment, quint32 parentId)
{
    //qDebug( ) << __PRETTY_FUNCTION__ << "Added BodySegment with parentId" << parentId;
    //BodySegment *bs=bodySegment.get();
    m_bodySegments.insert(bodySegment.get()->getId(),bodySegment.get());
    if(parentId==0) {
        osg::PositionAttitudeTransform *trans=m_bodyRoot.get();
        trans->addChild(bodySegment);
        m_bodyRoot.get()->addChild(bodySegment.get());
        return;
    }
    BodySegment *bs=m_bodySegments.value(parentId,0);
    if(bs) {
        bs->addChild(bodySegment);
    } else {
        qCritical() << __PRETTY_FUNCTION__ << "Unknown parentId " << parentId;
    }
}

void Body::advance()
{
    foreach(BodySegment *seg,m_bodySegments) {
        seg->computeRestMatrix();
    }
}

void Body::applyPlayers()
{
    m_animationPlayer->apply();
    for(quint32 i=0; i<MotionTreeManager::NUM_OF_TREE_TRACKS;i++) {
       m_treeAnimationPlayers.at(i)->apply();
    }
}

Agent* Body::getAgent()
{
    return m_agent;
}

QHash<QString,Animation *> * Body::getAnimations()
{
    return m_animationPlayer->getAnimations();
}

void Body::highlightSegment(quint32 id, bool unselectOthers)
{
    if(unselectOthers) {
        foreach(BodySegment *seg, m_bodySegments) {
            if(seg->getId()!=id) {
                seg->highlight(false);
            } else {
                seg->highlight(true);
            }
        }
    }
}

void Body::reset()
{
    foreach(BodySegment *seg, m_bodySegments) {
        seg->reset();
    }
    this->updatePosition();
    m_animationPlayer->reset();
    for(quint32 i=0; i<MotionTreeManager::NUM_OF_TREE_TRACKS;i++) {
       m_treeAnimationPlayers.at(i)->reset();
    }
}

void Body::setAnimations(QHash<QString, Animation *> *animations)
{
    m_animationPlayer->setAnimations(animations);
    for(quint32 i=0; i<MotionTreeManager::NUM_OF_TREE_TRACKS;i++) {
       m_treeAnimationPlayers.at(i)->setAnimations(animations);
    }
    foreach(Animation *anim, *m_animationPlayer->getAnimations()) {
        Channel *oChan=new Channel(0.0,2.0);
        m_agent->addOutputChannel(oChan,anim->name());

        Channel *iChan=new Channel(0.0,1.0);
        m_agent->addInputChannel(iChan,anim->name());

        QString phaseOffsetChannelName=anim->name().append(BrainiacGlobals::ChannelPhaseOffsetSuffix);
        Channel *phaseOffset=new Channel(0.0,1.0);
        m_agent->addOutputChannel(phaseOffset,phaseOffsetChannelName);
    }
}

void Body::showPivotCoordCrosses(bool show)
{
    foreach(BodySegment *seg, m_bodySegments) {
        seg->showPivotCoordCross(show);
    }
}

void Body::toggleShowCoordCrosses()
{
    m_showAllCoordCrosses=!m_showAllCoordCrosses;
    this->showPivotCoordCrosses(m_showAllCoordCrosses);
}

void Body::toggleShowPositionMarker()
{
    m_showPositionMarker=!m_showPositionMarker;
    m_switchPositionMarker->setValue(0,m_showPositionMarker);
}

void Body::toggleShowSkeleton()
{
    m_showSkeleton=!m_showSkeleton;
}

void Body::updatePosition() {
    if(m_agent) {
       osg::PositionAttitudeTransform *posTrans=m_bodyRoot.get();
       osg::Quat q;
       q.set(0,1,0,m_agent->getRotation()->y());//!< @todo Rotation order and other axis
       posTrans->setPosition(osg::Vec3d(m_agent->getPosition().x(),m_agent->getPosition().y(),m_agent->getPosition().z()));
       posTrans->setAttitude(q);
    }
}

Body::~Body()
{
    m_agent->getScene()->getRootSceneNode()->removeChild(m_bodyRoot);
}
