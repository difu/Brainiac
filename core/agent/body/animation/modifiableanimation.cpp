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


#include "modifiableanimation.h"

#include <QDebug>
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"
#include "core/agent/body/animation/animationcurve.h"
#include "cml/cml.h"
#include "cml/mathlib/matrix_rotation.h"

ModifiableAnimation::ModifiableAnimation( Animation *animation, Body *body) : Animation(animation)
{
    m_startTime=0.0f;
    m_endTime=getLength(true);
    m_crossFadeTime=0;
    m_body=body;
    m_origAnimation=(animation);
    setAnimationType(m_animType);
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
        delete origCuve;
    }
    QWriteLocker wLocker(&m_rwLock);
    Q_UNUSED(wLocker);
    //m_curves.clear();
    m_curves=newCurves;
    qDebug() << __PRETTY_FUNCTION__ ;
//    m_origAnimation->copyFromAnimationCurves(m_curves);
//    m_origAnimation->setIsLoopedAnimation(m_isLoopedAnimation);
//    m_origAnimation->setIsRetriggerable(m_isRetriggerable);
    m_origAnimation->copyFromAnimation(this);
    //setAnimationType(BrainiacGlobals::NOTYPE);
    wLocker.unlock();
    m_startTime=0.0f;
    m_crossFadeTime=0.0f;
    m_endTime=getLength(true);

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
    if(!hasAgentCurves()) {
        qDebug() << __PRETTY_FUNCTION__ << "AgentRootCurves already exist! nothing is created";
    } else {
        AnimationCurve *txCurve;
        AnimationCurve *tzCurve;

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

        switch(m_animType) {
        case BrainiacGlobals::LOCOMOTION:
            qDebug() << __PRETTY_FUNCTION__ << "creating LOCO curves";
            txCurve=new AnimationCurve();
            tzCurve=new AnimationCurve();
            createAgentCurve(rootBoneTxCurve,txCurve);
            createAgentCurve(rootBoneTzCurve,tzCurve);
            m_curves.insert("tx",txCurve);
            m_curves.insert("tz",tzCurve);
            break;
        default:
            qWarning() << __PRETTY_FUNCTION__ << "Animation type " << m_animType <<" not implemented!"; ;
            break;
        }
    }
    if(!m_transitionCurve) {
        m_transitionCurve=new AnimationCurve();
    }
    if(m_latchCurves.isEmpty()) {

    } else {
        qDebug() << __PRETTY_FUNCTION__ << "Latches already exist! nothing is created";
    }
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

void ModifiableAnimation::setTansformRotation(qreal yAxisRot)
{
    yAxisRot=-yAxisRot;
    QWriteLocker wLocker(&m_rwLock);
    Q_UNUSED(wLocker);
    bool isPositiveYAngle=false;
    if(yAxisRot>0.0f) {
        isPositiveYAngle=true;
    }
    SkeletonNode *rootBone=m_body->getRootBone();

    AnimationCurve *rootBoneTxCurve=curves().value(rootBone->getName()%":tx",0);
    if(!rootBoneTxCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No TX root bone curve!";
        return;
    }
    AnimationCurve *origRootBoneTxCurve=m_origAnimation->curves().value(rootBone->getName()%":tx",0);
    if(!origRootBoneTxCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No original TX root bone curve!";
        return;
    }


    AnimationCurve *rootBoneTyCurve=curves().value(rootBone->getName()%":ty",0);
    if(!rootBoneTyCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No TY root bone curve!";
        return;
    }
    AnimationCurve *origRootBoneTyCurve=m_origAnimation->curves().value(rootBone->getName()%":ty",0);
    if(!origRootBoneTyCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No original TY root bone curve!";
        return;
    }

    AnimationCurve *rootBoneTzCurve=curves().value(rootBone->getName()%":tz",0);
    if(!rootBoneTzCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No TZ root bone curve!";
        return;
    }
    AnimationCurve *origRootBoneTzCurve=m_origAnimation->curves().value(rootBone->getName()%":tz",0);
    if(!origRootBoneTzCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No original TZ root bone curve!";
        return;
    }


    AnimationCurve *rootBoneRxCurve=curves().value(rootBone->getName()%":rx",0);
    if(!rootBoneRxCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No RX root bone curve!";
        return;
    }
    AnimationCurve *origRootBoneRxCurve=m_origAnimation->curves().value(rootBone->getName()%":rx",0);
    if(!origRootBoneRxCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No original RX root bone curve!";
        return;
    }

    AnimationCurve *rootBoneRyCurve=curves().value(rootBone->getName()%":ry",0);
    if(!rootBoneRyCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No RY root bone curve!";
        return;
    }
    AnimationCurve *origRootBoneRyCurve=m_origAnimation->curves().value(rootBone->getName()%":ry",0);
    if(!origRootBoneRyCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No original RY root bone curve!";
        return;
    }

    AnimationCurve *rootBoneRzCurve=curves().value(rootBone->getName()%":rz",0);
    if(!rootBoneRzCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No RZ root bone curve!";
        return;
    }
    AnimationCurve *origRootBoneRzCurve=m_origAnimation->curves().value(rootBone->getName()%":rz",0);
    if(!origRootBoneRzCurve) {
        qWarning() << __PRETTY_FUNCTION__ <<"No original RZ root bone curve!";
        return;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Translation Curves
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QQuaternion rotQuatY=QQuaternion::fromAxisAndAngle(0.0f,1.0f,0.0f,yAxisRot+180.0f);
    rotQuatY.normalize();
    QVector3D origin;
    origin.setX(origRootBoneTxCurve->getValue(0.0f));
    origin.setY(origRootBoneTyCurve->getValue(0.0f));
    origin.setZ(origRootBoneTzCurve->getValue(0.0f));

    for(int i=1;i<rootBoneTzCurve->keyFrames().count();i++) {
        QVector3D originalPoint(origRootBoneTxCurve->getValue(rootBoneTxCurve->keyFrames().at(i).x()),
                                origRootBoneTyCurve->getValue(rootBoneTyCurve->keyFrames().at(i).x()),
                                origRootBoneTzCurve->getValue(rootBoneTzCurve->keyFrames().at(i).x()));
        QVector3D point=origin-originalPoint;
        QVector3D rotated=rotQuatY.rotatedVector(point);
        rootBoneTxCurve->keyFrames()[i].setY((rotated.x()+origin.x()));
        //rootBoneTyCurve->keyFrames()[i].setY((rotated.y()+origin.y()));
        rootBoneTzCurve->keyFrames()[i].setY((rotated.z()+origin.z()));
        //qDebug() << __PRETTY_FUNCTION__ << yAxisRot << rotated+origin << originalPoint;
    }



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Rotation Curves
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QList<BrainiacGlobals::RotTrans> rotList;
    foreach(BrainiacGlobals::RotTrans rot,rootBone->getRotationTranslationOrder()) {
        switch( rot ) {
        case BrainiacGlobals::RX:
        case BrainiacGlobals::RY:
        case BrainiacGlobals::RZ:
            rotList.append(rot);
        break;
        default:
            continue;
        }
    }

    QVector3D lastOrigVec;
    QVector3D lastCMLVec;
    for(int i=0;i<rootBoneRyCurve->keyFrames().count();i++) {
//        QMatrix4x4 rotMatrix;
//        rotMatrix.setToIdentity();

        cml::matrix33d_r cmlRotMatrix;
        cmlRotMatrix.identity();

        QVector3D originalRot  (origRootBoneRxCurve->getValue(rootBoneRxCurve->keyFrames().at(i).x()),
                                origRootBoneRyCurve->getValue(rootBoneRyCurve->keyFrames().at(i).x()),
                                origRootBoneRzCurve->getValue(rootBoneRzCurve->keyFrames().at(i).x()));

        qDebug() << __PRETTY_FUNCTION__ << "KF: "<< i << " Original Rotation Vector: " << originalRot << yAxisRot;
        cml::EulerOrder cmlEulerOrder;
        Q_ASSERT(rotList.count()==3);
        if(rotList.first()==BrainiacGlobals::RX && rotList.at(1)==BrainiacGlobals::RY && rotList.last()==BrainiacGlobals::RZ) {
            cmlEulerOrder=cml::euler_order_xyz;
        } else if(rotList.first()==BrainiacGlobals::RX && rotList.at(1)==BrainiacGlobals::RZ && rotList.last()==BrainiacGlobals::RY) {
            cmlEulerOrder=cml::euler_order_xzy;
        } else if(rotList.first()==BrainiacGlobals::RZ && rotList.at(1)==BrainiacGlobals::RY && rotList.last()==BrainiacGlobals::RX) {
            cmlEulerOrder=cml::euler_order_zyx;
        } else if(rotList.first()==BrainiacGlobals::RZ && rotList.at(1)==BrainiacGlobals::RX && rotList.last()==BrainiacGlobals::RY) {
            cmlEulerOrder=cml::euler_order_zxy;
        } else if(rotList.first()==BrainiacGlobals::RY && rotList.at(1)==BrainiacGlobals::RZ && rotList.last()==BrainiacGlobals::RX) {
            cmlEulerOrder=cml::euler_order_yzx;
        } else if(rotList.first()==BrainiacGlobals::RY && rotList.at(1)==BrainiacGlobals::RX && rotList.last()==BrainiacGlobals::RZ) {
            cmlEulerOrder=cml::euler_order_yxz;
        } else {
            qCritical() << __PRETTY_FUNCTION__ << "This never should happen!";
        }

        float xRot,yRot,zRot;

        cml::matrix_rotation_euler(cmlRotMatrix,
                                   BrainiacGlobals::grad2rad(originalRot.x()),
                                   BrainiacGlobals::grad2rad(originalRot.y()),
                                   BrainiacGlobals::grad2rad(originalRot.z()),
                                   cml::euler_order_xyz);

        cml::matrix_rotate_about_world_y(cmlRotMatrix,BrainiacGlobals::grad2rad(yAxisRot));

        cml::matrix_to_euler(cmlRotMatrix,xRot,yRot,zRot,cml::euler_order_xyz);
        qDebug() << __PRETTY_FUNCTION__ << "KF: "<< i << " Rotation CML:                       " << BrainiacGlobals::rad2grad(xRot) << BrainiacGlobals::rad2grad(yRot) << BrainiacGlobals::rad2grad(zRot);

        rootBoneRxCurve->keyFrames()[i].setY(BrainiacGlobals::rad2grad(xRot));
        rootBoneRyCurve->keyFrames()[i].setY(BrainiacGlobals::rad2grad(yRot));
        rootBoneRzCurve->keyFrames()[i].setY(BrainiacGlobals::rad2grad(zRot));

//        qDebug() << "DIFFS:" << originalRot-lastOrigVec << QVector3D(BrainiacGlobals::rad2grad(xRot-lastCMLVec.x()),BrainiacGlobals::rad2grad(yRot-lastCMLVec.y()),BrainiacGlobals::rad2grad(zRot-lastCMLVec.z()));
//        lastOrigVec=originalRot;
//        lastCMLVec=QVector3D(xRot,yRot,zRot);
    }

//    SkeletonNode *rootSkeletonNode=m_body->getRootSkeletonNode();
//    rootSkeletonNode->setRestRotation(QVector3D(0.0f,rot,0.0f));
}

ModifiableAnimation::~ModifiableAnimation(){

}
