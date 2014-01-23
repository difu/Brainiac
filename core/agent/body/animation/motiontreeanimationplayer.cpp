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

#include "motiontreeanimationplayer.h"
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

MotionTreeAnimationPlayer::MotionTreeAnimationPlayer(Body *body, MotionTree *tree, quint32 id) : AnimationPlayer(body), m_motionTree(tree), m_id(id)
{

    QString phaseChannelName=QString(BrainiacGlobals::ChannelName_Phase);
    QString latchChannelName=QString(BrainiacGlobals::ChannelName_Latch);
    if(m_id>0) {
        phaseChannelName.append(QString::number(m_id+1));
        latchChannelName.append(QString::number(m_id+1));
    }
    m_phaseChannel=new Channel(0,1);
    m_body->getAgent()->addInputChannel(m_phaseChannel,phaseChannelName);
    m_latchChannel=new Channel(0,1);
    m_body->getAgent()->addInputChannel(m_latchChannel,latchChannelName);

    reset();
}

void MotionTreeAnimationPlayer::apply()
{
    if(m_currentAnimation==0) {
        m_currentAnimation=m_animations->value(m_motionTree->getDefaultActionName(),0);
        if(m_currentAnimation) {
            m_currentAnimationStartTime=m_simulation->getCurrentTime();
        }
    }

    // Apply current Animation
    if(m_currentAnimation) {
        qreal animLength=m_currentAnimation->getLength();
        qreal offset=Channel::getOutputValue(m_body->getAgent(),m_currentAnimation->name().append(BrainiacGlobals::ChannelPhaseOffsetSuffix))*animLength;
        qreal diffTime=m_simulation->getCurrentTime()-m_currentAnimationStartTime+offset;
        if(m_currentAnimation->isLoopedAnimation()) {
            qreal loopTime=m_currentAnimation->getLoopAnimationTime(diffTime);
            AnimationPlayer::apply(*m_currentAnimation,loopTime);
            m_phaseChannel->setValue(loopTime/animLength);
        } else {
            AnimationPlayer::apply(*m_currentAnimation,diffTime);
            m_phaseChannel->setValue(diffTime/animLength);
        }
        m_body->getAgent()->getInputChannel(m_currentAnimation->name())->setValue(1.0);
    }

}

void MotionTreeAnimationPlayer::reset()
{
    //qDebug() << __PRETTY_FUNCTION__;
    AnimationPlayer::reset();
}
