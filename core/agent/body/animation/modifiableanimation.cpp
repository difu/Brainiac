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

}

qreal ModifiableAnimation::getValue(const QString &curve, qreal time) const
{
    if(qFuzzyCompare(m_crossFadeTime,(qreal)0.0f)) {
        return Animation::getValue(curve,time);
    } else if(time>=0.0f) {

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

QVector3D ModifiableAnimation::getRootSkeletonNodeTranslation(qreal time) const
{
    return QVector3D();
}

bool ModifiableAnimation::hasRootCurves() const
{
    return false;
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
