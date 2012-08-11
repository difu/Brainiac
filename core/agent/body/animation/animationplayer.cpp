#include "animationplayer.h"

#include "core/simulation.h"
#include "core/agent/body/body.h"
#include "core/agent/agent.h"
#include "core/agent/channel.h"
#include "core/scene.h"
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
    m_time=0;
}

void AnimationPlayer::apply2(const Animation &animation, qreal val)
{
    qreal boundVal=qBound((qreal)0.0f,val,(qreal)1.0f);
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

void AnimationPlayer::setAnimations(QHash<quint32, Animation *> *animations)
{
    m_animations=animations;
}
