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
#include "core/brainiaclogger.h"

MotionTreeAnimationPlayer::MotionTreeAnimationPlayer(Body *body, MotionTree *tree) : AnimationPlayer(body), m_motionTree(tree)
{
    qCDebug(bAnimation) << __PRETTY_FUNCTION__ << "Creating phases and latch channels of track " << m_motionTree->getTrack();
    m_phaseChannel=new Channel(m_body->getAgent(),0,1);
    m_body->getAgent()->addInputChannel(m_phaseChannel,BrainiacGlobals::ChannelNames_Phases.at(m_motionTree->getTrack()));
    m_latchChannel=new Channel(m_body->getAgent(),0,1);
    m_body->getAgent()->addInputChannel(m_latchChannel,BrainiacGlobals::ChannelNames_Latches.at(m_motionTree->getTrack()));

    reset();
}

void MotionTreeAnimationPlayer::apply()
{
    MotionTreeAction *currentAction=0;
    if(m_currentAnimation==0) {
        m_currentAnimation=m_body->getAgent()->getAgentManager()->getAnimations()->value(m_motionTree->getDefaultActionName(),0);
        if(m_currentAnimation) {
            m_currentAnimationStartTime=m_simulation->getCurrentTime();
        }
    }

    // Apply current Animation

    bool hasLatch=true;
    if(m_currentAnimation) {
        currentAction=m_motionTree->getActions().value(m_currentAnimation->name(),0);
        if(!currentAction) {
            qCDebug(bAnimation) << __PRETTY_FUNCTION__ << "Action does not match Animation " << m_currentAnimation->name();
        }
        qreal animLength=m_currentAnimation->getLength();
        qreal offset=Channel::getOutputValue(m_body->getAgent(),m_currentAnimation->getPhaseOffsetChannelName())*animLength;
        qreal diffTime=m_simulation->getCurrentTime()-m_currentAnimationStartTime+offset;
        qreal animTime=0.0;
        if(m_currentAnimation->isLoopedAnimation()) {
            qreal loopTime=m_currentAnimation->getLoopAnimationTime(diffTime);
            m_phaseChannel->setValue(loopTime/animLength);
            animTime=loopTime;
        } else {
            m_phaseChannel->setValue(diffTime/animLength);
            animTime=diffTime;
            qCDebug(bAnimation) << __PRETTY_FUNCTION__ << "anim is not looped";
        }
        AnimationPlayer::apply(*m_currentAnimation,animTime);
        hasLatch=m_currentAnimation->hasLatch(animTime);
        m_latchChannel->setValue(hasLatch?1.0:0.0);
        Channel::setInputValue(m_body->getAgent(),m_currentAnimation->getRunningChannelName(),1.0,Channel::NONE_WARN);
    }

    // check for triggers of next possible actions
    // only check, if no next animation has been set
    if(!m_nextAnimation && !hasLatch) {
        MotionTreeManager *manager=(MotionTreeManager *)m_motionTree->parent();
        QString highestTriggerName;
        qreal highestValue=0;
        foreach(QString triggerName, manager->getTriggers()) {
            qreal val=Channel::getOutputValue(m_body->getAgent(),triggerName);
            if(val>=1.0 && val>highestValue) {
                highestValue=val;
                highestTriggerName=triggerName;
            }
        }
        if(highestValue>=BrainiacGlobals::ActionTriggerValue) {
            if(currentAction) {
                MotionTreeAction *nextAction=0;
                if( m_motionTree->getNextActionsFromAction(currentAction).isEmpty()) {
                    qCDebug(bAnimation) << __PRETTY_FUNCTION__ << "No next action/animation found! current action: " << currentAction->getName();
                }
                foreach(MotionTreeAction *possibleNextaction, m_motionTree->getNextActionsFromAction(currentAction)) {
                    foreach(quint32 triggerId, possibleNextaction->getTriggerIds()) {
                        if(manager->getTriggers().value(triggerId,QString())==highestTriggerName) {
                            nextAction=possibleNextaction;
                            break;
                        }
                    }
                    if(nextAction) {
                        m_nextAnimation=m_body->getAgent()->getAgentManager()->getAnimations()->value(nextAction->getName());
                        qCDebug(bAnimation) << __PRETTY_FUNCTION__ << "Next action name: "<< nextAction->getName();
                        break;
                    }
                }
            }
        }
    }
}

void MotionTreeAnimationPlayer::reset()
{
    AnimationPlayer::reset();
}
