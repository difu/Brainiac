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


AnimationPlayer::AnimationPlayer(Body *body)
{
    m_body=body;
    m_simulation=body->getAgent()->getScene()->getSimulation();
    m_animDefault.addTransition(this,SIGNAL(animCanTransit()),&m_animCanTrans);
    m_animCanTrans.addTransition(this,SIGNAL(animIsInTransition()),&m_animInTransition);
    m_animCanTrans.addTransition(this,SIGNAL(animCannotTransit()),&m_animDefault);
    m_animInTransition.addTransition(this,SIGNAL(aninHasTransitioned()),&m_animDefault);
    m_stateMachine.addState(&m_animDefault);
    m_stateMachine.addState(&m_animCanTrans);
    m_stateMachine.addState(&m_animInTransition);
    m_stateMachine.setInitialState(&m_animDefault);
    m_stateMachine.start();
    reset();
}

void AnimationPlayer::apply()
{
    // first apply highest triggered action, will be possibly overwritten by motiontree
    qreal highestValue=0;
    Animation *highestAnimation=0;
    foreach(Animation *anim,*m_animations) {
        qreal tmpHv=Channel::getValue(m_body->getAgent(),anim->name())>highestValue;
        if(tmpHv>highestValue) {
            highestValue=tmpHv;
            highestAnimation=anim;
        }
    }

    // if no animation is running, just trigger the highest
    if(m_currentAnimation==0 && highestAnimation) {
        m_currentAnimation=highestAnimation;
        m_currentAnimationStartTime=m_simulation->getCurrentFrame()/(qreal)m_simulation->getFps();
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

    //animation.curves().value("ToSpine:ry")->dPrintKeyFrames(0,10);
}

void AnimationPlayer::reset()
{
    m_time=0;
    m_currentAnimation=0;
    m_currentAnimationStartTime=0;
    m_nextAnimation=0;
}

void AnimationPlayer::setAnimations(QHash<QString, Animation *> *animations)
{
    m_animations=animations;
}
