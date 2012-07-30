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
}

void AnimationPlayer::apply2(const Animation &animation, qreal val)
{
    qreal boundVal=qBound((qreal)0.0f,val,(qreal)1.0f);
    qreal length=10.0f;
    apply(animation,length*boundVal);
}

void AnimationPlayer::apply(const Animation &animation, qreal time)
{
//    if(animation.curves().count()>0) {
//        QHashIterator<QString, AnimationCurve *> i(animation.curves()) ;
//        //m_body->getAgent()->dDumpChannels();
//        while(i.hasNext()) {
//            i.next();
//            //qDebug() << __PRETTY_FUNCTION__ << "applying for channel" << i.key();
//            Channel *c=m_body->getAgent()->getOutputChannel(i.key());
//            if(c) {
//                m_body->getAgent()->getOutputChannel(i.key())->setValue(animation.getValue(i.key(),time));
//                //qDebug() << __PRETTY_FUNCTION__ << "applying for channel" << i.key()  << i.value()->getValue(time);
//            } else {
//                qDebug() << __PRETTY_FUNCTION__ << "Channel " << i.key() << "not found!";
//            }
//        }
//    }
    foreach(QString curveName,animation.curveNames()) {

        Channel *c=m_body->getAgent()->getOutputChannel(curveName);
        if(c) {
            //qDebug() << __PRETTY_FUNCTION__ << "Applying "<< curveName;
//            if(curveName=="tx" || curveName=="tz") {
//                qDebug() << __PRETTY_FUNCTION__ << "Applying "<< curveName << time << animation.getValue(curveName,time) ;
//            }
            c->setValue(animation.getValue(curveName,time));
        }
    }

    //animation.curves().value("ToSpine:ry")->dPrintKeyFrames(0,10);
}

void AnimationPlayer::setAnimations(QHash<quint32, Animation *> *animations)
{
    m_animations=animations;
}
