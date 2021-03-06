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
#include "core/brainiaclogger.h"

#include <QColor>



Body::Body(Agent *agent)
{
    m_agent=agent;
    m_animationPlayer=new AnimationPlayer(this);
    for(quint32 i=0; i<MotionTreeManager::NUM_OF_TREE_TRACKS;i++) {
        MotionTreeAnimationPlayer *player=new MotionTreeAnimationPlayer(this,m_agent->getAgentManager()->getMotionTreeManager()->getMotionTrees().value(i));
        m_treeAnimationPlayers.append(player);
    }
    qCDebug(bAnimation) << __PRETTY_FUNCTION__ << "Create trigger outputs";
    foreach(QString triggerName, m_agent->getAgentManager()->getMotionTreeManager()->getTriggers()) {
        Channel *tc=m_agent->getOrCreateOutputChannel(triggerName);
        tc->setMinValue(0.0);
        tc->setMaxValue(BrainiacGlobals::ActionForceTriggerValue);
    }
    qCDebug(bAnimation) << __PRETTY_FUNCTION__ << "Create animations";
    foreach(Animation *anim, *getAgent()->getAgentManager()->getAnimations()) {
        this->addAnimation(anim);
    }
    m_bodyRoot=new osg::PositionAttitudeTransform;
    m_bodyRoot.get()->setName("AgentBody Root Segment");
    m_showAllCoordCrosses=false;
    m_showSkeleton=false;
    m_switchPositionMarker=new osg::Switch;
    osg::Geode *geo=new osg::Geode;
    geo->addDrawable(BrainiacGlobals::CoordCross.get());

    // LOD
    m_lodBody=new osg::LOD;
    m_lodBody->setRangeMode( osg::LOD::DISTANCE_FROM_EYE_POINT );
    static float _LodDistance1=10000.f;

    // Proxy
    m_proxyBody=new osg::Group;
    m_realBody=new osg::Group;
    m_switchProxy=new osg::Switch;
    m_bodyRoot->addChild(m_switchProxy);
    m_lodBody->addChild(m_realBody,0.0f, _LodDistance1);
    {
        osg::Geode *geo2=new osg::Geode;
        geo2->addDrawable(BrainiacGlobals::CoordCross.get());
        m_lodBody->addChild(geo2,_LodDistance1, 100000000.0f);
    }
    m_switchProxy->addChild(m_lodBody,true);
    m_switchProxy->addChild(m_proxyBody,false);

    m_switchSkeleton=new osg::Switch;
    m_bodyRoot->addChild(m_switchSkeleton);

    // Switch Lightning off for position marker
    osg::StateSet *state = geo->getOrCreateStateSet();
    state->setMode( GL_LIGHTING, osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF );

    m_switchPositionMarker->addChild(geo,false);
    m_bodyRoot->addChild(m_switchPositionMarker);
    m_showPositionMarker=false;
}

void Body::addAnimation(Animation *animation)
{
    qCDebug(bAnimation) << __PRETTY_FUNCTION__ << "Create animation channel " << animation->name();
    Channel *oChan=m_agent->getOrCreateOutputChannel(animation->name());
    oChan->setMinValue(0.0);
    oChan->setMaxValue(2.0);

    qCDebug(bAnimation) << __PRETTY_FUNCTION__ << "Create animation:running input channel " << animation->getRunningChannelName();
    Channel *iChan=new Channel(m_agent,0.0,1.0);
    m_agent->addInputChannel(iChan,animation->getRunningChannelName());

    qCDebug(bAnimation) << __PRETTY_FUNCTION__ << "Create animation:phase output channel " << animation->getPhaseOffsetChannelName();
    Channel *phaseOffset=m_agent->getOrCreateOutputChannel(animation->getPhaseOffsetChannelName());
    phaseOffset->setMinValue(0.0);
    phaseOffset->setMaxValue(1.0);
}

void Body::addBodySegment(osg::ref_ptr<BodySegment> bodySegment, quint32 parentId)
{
    //qDebug( ) << __PRETTY_FUNCTION__ << "Added BodySegment with parentId" << parentId;
    //BodySegment *bs=bodySegment.get();
    m_bodySegments.insert(bodySegment.get()->getId(),bodySegment.get());
    if(parentId==0) {
        //osg::PositionAttitudeTransform *trans=m_bodyRoot.get();
        //trans->addChild(bodySegment);
        m_realBody.get()->addChild(bodySegment.get());
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

void Body::showPositionMarker(bool show)
{
    m_showPositionMarker=show;
    m_switchPositionMarker->setValue(0,m_showPositionMarker);
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
    this->showPositionMarker(m_showPositionMarker);

}

void Body::toggleShowSkeleton()
{
    m_showSkeleton=!m_showSkeleton;
}

void Body::updatePosition() {
    if(m_agent) {
       osg::PositionAttitudeTransform *posTrans=m_bodyRoot.get();
       osg::Quat q;
       q.makeRotate(BrainiacGlobals::grad2rad(m_agent->getRotation().y()),osg::Vec3d(0.0,1.0,0.0));//!< @todo Rotation order and other axis
       posTrans->setPosition(osg::Vec3d(m_agent->getPosition().x(),m_agent->getPosition().y(),m_agent->getPosition().z()));
       posTrans->setAttitude(q);
    }
}

Body::~Body()
{
    delete m_animationPlayer;
    foreach(MotionTreeAnimationPlayer *player, m_treeAnimationPlayers) {
        delete player;
    }

    m_agent->getScene()->getRootSceneNode()->removeChild(m_bodyRoot);
}
