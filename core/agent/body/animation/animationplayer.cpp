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


#include "animationplayer.h"

#include "core/simulation.h"
#include "core/agent/body/body.h"
#include "core/agent/agent.h"
#include "core/agent/channel.h"
#include "core/scene.h"
#include "core/simulation.h"
#include "core/agent/body/animation/animation.h"
#include "core/agent/body/animation/animationcurve.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/animation/motiontreemanager.h"
#include "core/agent/body/animation/motiontree.h"
#include "core/agent/body/animation/motiontreeaction.h"


AnimationPlayer::AnimationPlayer(Body *body)
{
    m_body=body;
    m_simulation=body->getAgent()->getScene()->getSimulation();

//    m_animDefault.addTransition(this,SIGNAL(animCanTransit()),&m_animCanTrans);
//    m_animCanTrans.addTransition(this,SIGNAL(animIsInTransition()),&m_animInTransition);
//    m_animCanTrans.addTransition(this,SIGNAL(animCannotTransit()),&m_animDefault);
//    m_animInTransition.addTransition(this,SIGNAL(aninHasTransitioned()),&m_animDefault);
//    m_stateMachine.addState(&m_animDefault);
//    m_stateMachine.addState(&m_animCanTrans);
//    m_stateMachine.addState(&m_animInTransition);
//    m_stateMachine.setInitialState(&m_animDefault);
//    m_stateMachine.start();
    reset();
}

void AnimationPlayer::apply()
{
    if(true) { // Just for testing
        // first apply highest triggered action, will be possibly overwritten by motiontrees
        qreal highestValue=0.0;
        Animation *highestAnimation=0;
        foreach(Animation *anim,*m_animations) {
            qreal tmpHv=Channel::getOutputValue(m_body->getAgent(),anim->name())>highestValue;
            if(tmpHv>highestValue) {
                highestValue=tmpHv;
                highestAnimation=anim;
            }
        }

        // if no animation is running, just trigger the highest
        if(m_currentAnimation==0 && highestAnimation) {
            m_currentAnimation=highestAnimation;
            m_currentAnimationStartTime=m_simulation->getCurrentTime();
            m_currentAnimationStartedForFirstTime=true;
        }

        // Apply current Animation
        if(m_currentAnimation) {
            qreal animLength=m_currentAnimation->getLength();
            qreal offset=Channel::getOutputValue(m_body->getAgent(),m_currentAnimation->name().append(BrainiacGlobals::ChannelPhaseOffsetSuffix))*animLength;
            qreal diffTime=m_simulation->getCurrentTime()-m_currentAnimationStartTime+offset;

            if(m_currentAnimation->isLoopedAnimation()) {
                qreal loopTime=m_currentAnimation->getLoopAnimationTime(diffTime);
                apply(*m_currentAnimation,loopTime);
            } else {
                apply(*m_currentAnimation,diffTime);
            }
            m_body->getAgent()->getInputChannel(m_currentAnimation->name())->setValue(1.0);
        }

    }


}

void AnimationPlayer::apply2(const Animation &animation, qreal val)
{
    qreal boundVal=qBound(0.0,val,1.0);
    qreal length=animation.getLength();
    apply(animation,length*boundVal);
}

void AnimationPlayer::apply(const Animation &animation, qreal time)
{
    foreach(QString curveName,animation.curveNames()) {

        Channel *c=m_body->getAgent()->getOutputChannel(curveName);
        if(c) {
            c->setValue(animation.getValue(curveName,time));
        }
    }
}

void AnimationPlayer::reset()
{
    m_currentAnimation=0;
    m_currentAnimationStartTime=0;
    m_nextAnimation=0;
    m_nextAnimationStartTime=0;
    m_currentAnimationStartedForFirstTime=false;
}

void AnimationPlayer::setAnimations(QHash<QString, Animation *> *animations)
{
    m_animations=animations;
}
