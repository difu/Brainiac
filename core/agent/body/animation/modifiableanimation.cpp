#include "modifiableanimation.h"

#include <QDebug>
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"
#include "core/agent/body/animation/animationcurve.h"

ModifiableAnimation::ModifiableAnimation( Animation *animation, Body *body) : Animation(animation)
{
    m_startTime=0.0f;
    m_endTime=getLength(true);
    m_crossFadeTime=0;
    m_body=body;
    m_origAnimation=(animation);
    setAnimationType(BrainiacGlobals::NOTYPE);
    qDebug() << __PRETTY_FUNCTION__ << "Root bone node " << m_body->getRootBone()->getName();
    qDebug() << __PRETTY_FUNCTION__ << "Startime" << m_startTime << "End Time" << m_endTime;
}

void ModifiableAnimation::bake()
{
//    QWriteLocker wLocker(&m_rwLock);
//    Q_UNUSED(wLocker);
    QHash<QString, AnimationCurve*> newCurves;
    foreach(QString curveName,curveNames()) {
        AnimationCurve *newCurve=new AnimationCurve();
        AnimationCurve *origCuve=m_curves.value(curveName,0);
        newCurve->addKeyFrame(0.0f,getValue(curveName,m_startTime));
        newCurve->addKeyFrame(m_endTime-m_startTime,this->getValue(curveName,m_endTime));
        foreach(QVector2D kf, origCuve->keyFrames()) {
            if(kf.x()>m_startTime && kf.x() < m_endTime) {
                newCurve->addKeyFrame(kf.x()-m_startTime,this->getValue(curveName,kf.x()));
                qDebug() << __PRETTY_FUNCTION__ << curveName << kf.x() << kf.y() << this->getValue(curveName,kf.x()) << Animation::getValue(curveName,kf.x());
            }
        }
        qDebug() << __PRETTY_FUNCTION__ << curveName <<" oldKF" << origCuve->keyFrames().count() << "NewKF" << newCurve->keyFrames().count();
        newCurves.insert(curveName,newCurve);
    }
    QWriteLocker wLocker(&m_rwLock);
    Q_UNUSED(wLocker);
    //m_curves.clear();
    m_curves=newCurves;
    qDebug() << __PRETTY_FUNCTION__ ;
    m_origAnimation->copyFromAnimationCurves(m_curves);
    //setAnimationType(BrainiacGlobals::NOTYPE);
    wLocker.unlock();
    m_startTime=0.0f;
    m_crossFadeTime=0.0f;
    m_endTime=getLength(true);


//    QHashIterator<QString, AnimationCurve*> i(m_curves) ;
//    while(i.hasNext()) {
//        i.next();
//        AnimationCurve *c=i.value();
//        c->addKeyFrame(m_startTime,getValue(i.key(),m_startTime));
//        c->addKeyFrame(m_endTime,getValue(i.key(),m_endTime));
//    }
}

void ModifiableAnimation::createAgentCurve(AnimationCurve *rootCurve, AnimationCurve *agentCurve)
{
    if(rootCurve->keyFrames().count()==0) {
        return;
    }
    qreal valBefore=rootCurve->keyFrames().first().y();
    int i=0;
    foreach(QVector2D kf,rootCurve->keyFrames()) {
        agentCurve->addKeyFrame(kf.x(),kf.y()-valBefore);
        valBefore=kf.y();
        //rootCurve->keyFrames()[i].setY(0.0f);
        i++;
    }
    // The first kf does is zero. Interpolate its value
    if(rootCurve->keyFrames().count()==2) {
        agentCurve->keyFrames()[0].setY(agentCurve->keyFrames().last().y());
    } else if( rootCurve->keyFrames().count()>2) {
        agentCurve->keyFrames()[0].setY(0.5f*agentCurve->keyFrames()[1].y()+0.5f*agentCurve->keyFrames().last().y());
    }
    rootCurve->keyFrames().clear();
}

bool ModifiableAnimation::createAgentCurves()
{
    if(hasAgentCurves()) {
        qDebug() << __PRETTY_FUNCTION__ << "AgentCurves already exist! nothing is created";
        return false;
    }

    QWriteLocker wLocker(&m_rwLock);
    Q_UNUSED(wLocker);
    SkeletonNode *rootBone=m_body->getRootBone();
    AnimationCurve *rootBoneTxCurve=curves().value(rootBone->getName()%":tx",0);
    if(!rootBoneTxCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No TX root bone curve!";
        return false;
    }
    AnimationCurve *rootBoneTyCurve=curves().value(rootBone->getName()%":ty",0);
    if(!rootBoneTyCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No TY root bone curve!";
        return false;
    }
    AnimationCurve *rootBoneTzCurve=curves().value(rootBone->getName()%":tz",0);
    if(!rootBoneTzCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No TZ root bone curve!";
        return false;
    }
    AnimationCurve *txCurve=new AnimationCurve();
    AnimationCurve *tyCurve=new AnimationCurve();
    AnimationCurve *tzCurve=new AnimationCurve();
    AnimationCurve *rxCurve=new AnimationCurve();
    AnimationCurve *ryCurve=new AnimationCurve();
    AnimationCurve *rzCurve=new AnimationCurve();
    switch(m_animType) {
    case BrainiacGlobals::LOCOMOTION:
        qDebug() << __PRETTY_FUNCTION__ << "creating LOCO curves";
        createAgentCurve(rootBoneTxCurve,txCurve);
        createAgentCurve(rootBoneTzCurve,tzCurve);
        break;
    default:
        break;
    }
    m_curves.insert("tx",txCurve);
    m_curves.insert("ty",tyCurve);
    m_curves.insert("tz",tzCurve);
    m_curves.insert("rx",rxCurve);
    m_curves.insert("ry",ryCurve);
    m_curves.insert("rz",rzCurve);
    return true;
}

qreal ModifiableAnimation::getValue(const QString &curve, qreal time) const
{
    if(qFuzzyCompare(m_crossFadeTime,(qreal)0.0f)) {
        return Animation::getValue(curve,time);
    } else if(time>=0.0f) {
        QReadLocker rLocker(&m_rwLock);
        Q_UNUSED(rLocker);
        // Do we have a root bone curve?
        // if yes, check if it has to be crossfaded
        if( isRootBoneCurve(curve) ) {
            if((curve.endsWith(":rx") && m_crossFadeRx) || (curve.endsWith(":ry") && m_crossFadeRy) || (curve.endsWith(":rz") && m_crossFadeRz) ||
               (curve.endsWith(":tx") && m_crossFadeTx) || (curve.endsWith(":ty") && m_crossFadeTy) || (curve.endsWith(":tz") && m_crossFadeTz)) {
                qDebug()<< __PRETTY_FUNCTION__ << "Crossfade root curve "  << curve;
                return  getCrossfadedValue(curve,time);
            } else {
                return Animation::getValue(curve,time);
            }
        } else { // it is another curve, crossfade it anyway
            return getCrossfadedValue(curve,time);
        }
    }
    qDebug()<<__PRETTY_FUNCTION__ << "Time " << time << " is out of cf range! Curve "<< curve << "Start: " << m_startTime << "End: "<<m_endTime;
    return 0.0f;

}

qreal ModifiableAnimation::getCrossfadedValue(const QString &curve, qreal time) const
{
    if(m_startTime+m_crossFadeTime<time && m_endTime+m_crossFadeTime>time) { // are we out of crossfade range?
        //qDebug()<<__PRETTY_FUNCTION__ << "Out of cf range time" << time << "Start " << m_startTime << " End " << m_endTime << " CF " << m_crossFadeTime;
        return Animation::getValue(curve,time); // return non-crossfaded value
    }
    qreal diff=time-m_startTime;
    qreal origValue=Animation::getValue(curve,time);
    qreal cfValue; // Value at crossfade point
    if(m_crossFadeTime>0) {
        cfValue=Animation::getValue(curve,m_endTime+diff);
        qreal ratio=(diff/m_crossFadeTime);
        ratio+=0.1f; // @bug workaround, to prevent first frame==last frame
        if(ratio>1.0f) {
            return Animation::getValue(curve,time);
        }
        qreal interpolated=((ratio)*origValue+(1.0f-ratio)*cfValue);
        if(curve=="lLeg:rz")
            qDebug()<<__PRETTY_FUNCTION__ << curve << "origValue" << origValue << "cfVal " << cfValue << "@time:" << m_endTime+diff << "Interpolated " << interpolated << "Ratio " << ratio;
        return interpolated;
    }
    qDebug()<<__PRETTY_FUNCTION__ << "Not yet implementet cf <0";
}

QVector3D ModifiableAnimation::getRootBoneTranslation(qreal time) const
{
    SkeletonNode *rootBone=m_body->getRootBone();
    if(rootBone) {
        qreal x=getValue(rootBone->objectName()%":tx",time);
        qreal y=getValue(rootBone->objectName()%":ty",time);
        qreal z=getValue(rootBone->objectName()%":tz",time);
        //qDebug() << __PRETTY_FUNCTION__ << x<< y<< z << rootBone->objectName();
        return QVector3D(x,y,z);
    }
    return QVector3D(0,0,0);
}

bool ModifiableAnimation::isRootBoneCurve(const QString &curve) const
{
    SkeletonNode *rootBone=m_body->getRootBone();
    if(rootBone) {
        return curve==rootBone->objectName()%":tx" || curve==rootBone->objectName()%":ty" || curve==rootBone->objectName()%":tz"
            || curve==rootBone->objectName()%":rx" || curve==rootBone->objectName()%":ry" || curve==rootBone->objectName()%":rz";
    } else {
        return false;
    }

}

void ModifiableAnimation::resetAgentCurves()
{
    deleteCurves();
    copyFromAnimation(m_origAnimation);
}

void ModifiableAnimation::setAnimationType(BrainiacGlobals::AnimationType type)
{
    m_animType=type;
    switch(m_animType) {
    case BrainiacGlobals::STATIC:
        setCrossFadeRootCurves(true,true,true,true,true,true);
        break;
    case BrainiacGlobals::LOCOMOTION:
        setCrossFadeRootCurves(true,true,true,false,true,false);
        break;
    default:
        setCrossFadeRootCurves(false,false,false,false,false,false);
        break;
    }
}

void ModifiableAnimation::setCrossFade(qreal crossFade)
{
    if(m_startTime+crossFade<0.0f ) {
        m_crossFadeTime=-m_startTime;
    } else if(m_endTime+crossFade > getLength(true)) {
        m_crossFadeTime=getLength()-m_endTime;
    } else {
        m_crossFadeTime=crossFade;
    }
}

void ModifiableAnimation::setCrossFadeRootCurves(bool rx, bool ry, bool rz, bool tx, bool ty, bool tz)
{
    m_crossFadeRx=rx;
    m_crossFadeRy=ry;
    m_crossFadeRz=rz;
    m_crossFadeTx=tx;
    m_crossFadeTy=ty;
    m_crossFadeTz=tz;
}

void ModifiableAnimation::setStartTime(qreal startTime)
{
    m_startTime=qBound((qreal)0.0f,startTime,m_endTime-minDistTime);
}

void ModifiableAnimation::setEndTime(qreal endTime)
{
    m_endTime=qBound(m_startTime+minDistTime,endTime,getLength(true));
}

ModifiableAnimation::~ModifiableAnimation(){

}
