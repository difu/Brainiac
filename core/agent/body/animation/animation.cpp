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


#include "animation.h"
#include "core/agent/body/animation/animationcurve.h"
#include "core/agent/body/animation/latchcurve.h"
#include "core/brainiacglobals.h"

#include <QFile>
#include <QVector3D>
#include <QDebug>

Animation::Animation()
{
   calculateLength();
   m_transitionCurve=0;
}

Animation::Animation( Animation *animation)
{
    copyFromAnimation(animation);
}

Animation::Animation(QHash<QString, AnimationCurve *> curves, QString name="")
{
    setName(name);
    m_curves=curves;
    m_isLoopedAnimation=true;
    m_isRetriggerable=false;
    m_animType=BrainiacGlobals::STATIC;
    m_transitionCurve=0;
    calculateLength();
}

void Animation::addCurve(const QString &curveName)
{
    QWriteLocker wLocker(&m_rwLock);
    Q_UNUSED(wLocker);
    AnimationCurve *c=new AnimationCurve();
    m_curves.insert(curveName,c);
    wLocker.unlock();
    calculateLength();
}

void Animation::addCurve(const QString &curveName, AnimationCurve curve)
{
    QWriteLocker wLocker(&m_rwLock);
    Q_UNUSED(wLocker);
    AnimationCurve *c=new AnimationCurve(&curve);
    m_curves.insert(curveName,c);
    wLocker.unlock();
    calculateLength();
}

void Animation::addLatchCurve(const QString &latchName)
{
    QWriteLocker wLocker(&m_rwLock);
    Q_UNUSED(wLocker);
    LatchCurve *curve = new LatchCurve();
    m_latchCurves.insert(latchName,curve);
}

void Animation::addLatch(const QString &latchName, qreal start, qreal length)
{
    QWriteLocker wLocker(&m_rwLock);
    Q_UNUSED(wLocker);
    LatchCurve *c=m_latchCurves.value(latchName,0);
    if(c) {
        c->addLatch(start,length);
    } else {
        qDebug() << __PRETTY_FUNCTION__ << "latch with name " << latchName << "not found!";
    }
}

void Animation::addLatch(const QString &latchName, QVector2D latch)
{
    addLatch(latchName,latch.x(),latch.y());
}

void Animation::copyFromAnimation(Animation *animation)
{
    deleteCurves();
    QHashIterator<QString, AnimationCurve *> i(animation->curves()) ;

    while(i.hasNext()) {
        i.next();
        AnimationCurve *c=i.value();
        AnimationCurve *newCurve= new AnimationCurve(c);
        m_curves.insert(i.key(),newCurve);
        newCurve->generateIndexes();
    }
    QHashIterator<QString, LatchCurve *> j(animation->latches());
    while(j.hasNext()) {
        j.next();
        LatchCurve *l=j.value();
        LatchCurve *newLatchCurve=new LatchCurve(*l);
        m_latchCurves.insert(j.key(),newLatchCurve);
    }
    calculateLength();
    setName(animation->name());
    m_fileName=animation->fileName();
    m_isLoopedAnimation=animation->isLoopedAnimation();
    m_isRetriggerable=animation->isRetriggerable();
    m_animType=animation->animationType();
    AnimationCurve *transCurve=animation->getTransitionCurve();
    if(transCurve) {
        m_transitionCurve=new AnimationCurve(transCurve);
    }
}

void Animation::calculateLength() const
{
    QReadLocker rLocker(&m_rwLock);
    Q_UNUSED(rLocker);
    m_length=0;

    foreach(AnimationCurve *c,m_curves) {
        if(c->keyFrames().isEmpty())
            continue;
        qreal time=c->keyFrames().last().x();
        if( time > m_length) {
            m_length=time;
        }
    }
}

QList<QString> Animation::curveNames() const
{
    QReadLocker rLocker(&m_rwLock);
    Q_UNUSED(rLocker);
    return m_curves.keys();
}

void Animation::deleteCurve(const QString &curveName)
{
    QWriteLocker wLocker(&m_rwLock);
    Q_UNUSED(wLocker);
    AnimationCurve *curve=m_curves.value(curveName,0);
    if(curve) {
        delete curve;
        m_curves.remove(curveName);
    }
}

void Animation::deleteCurves()
{
    QWriteLocker wLocker(&m_rwLock);
    Q_UNUSED(wLocker);
    foreach(AnimationCurve *curve,m_curves) {
        delete curve;
    }
    m_curves.clear();
    m_transitionCurve=0;
    foreach(LatchCurve *latch,m_latchCurves) {
        delete latch;
    }
    m_latchCurves.clear();
}

AnimationCurve* Animation::getTransitionCurve() const
{
    return m_transitionCurve;
}

qreal Animation::getLength(bool calculateNew) const
{
    if(calculateNew) {
        calculateLength();
    }
    return m_length;
}

qreal Animation::getLoopAnimationTime(qreal time) const
{
    qreal length=this->getLength();
    while( (time-length)>0 ) {
        time=time-length;
    }
    return time;
}

qreal Animation::getValue(const QString &curve, qreal time) const
{
    AnimationCurve *c=m_curves.value(curve,0);
    if(c) {
        return c->getValue(time);
    } else {
        qDebug() << __PRETTY_FUNCTION__ << "Curve " << curve << " not found";
        return 0.0;
    }

}

bool Animation::hasAgentCurves() const
{
    foreach(QString name, m_curves.keys()) {
        if(name=="rx" || name=="tx" ||
           name=="ry" || name=="ty" ||
           name=="rz" || name=="tz") {
            return true;
        }
    }
    return false;
}

bool Animation::hasLatch(qreal time) const
{
    foreach(LatchCurve *latch, m_latchCurves) {
        if(latch->isLatch(time)) {
            return true;
        } else {
            continue;
        }
    }
    return false;
}

void Animation::addKeyFrame(QString &curveName, qreal time, qreal value)
{
    QWriteLocker wLocker(&m_rwLock);
    Q_UNUSED(wLocker);
    AnimationCurve *curve=m_curves.value(curveName,0);
    if(curve) {
        curve->addKeyFrame(time,value);
    }
}


Animation* Animation::loadAnimation(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        qWarning() << __PRETTY_FUNCTION__ << "Could not open file "<< fileName << "for reading";
        return 0;
    }
    Animation *anim;
    QDataStream in(&file);
    quint32 magicNumber;
    qint32 version;
    in >> magicNumber >> version;
    if(magicNumber!=BrainiacGlobals::animationFileFormatMagicNumber || version!=BrainiacGlobals::animationFileFormatVersion) {
        qWarning() << __PRETTY_FUNCTION__ << "Error while reading " << fileName << ". Wrong file format";
        return 0;
    }
    QString name;
    in >> name;
    //anim->name()=name;
    int numOfCurves;
    in >> numOfCurves;
    QHash<QString, AnimationCurve*> animCurves;
    for(int i=0;i<numOfCurves;i++) {
        QString curveName;
        in >> curveName;
        int numOfKeyFrames;
        in >> numOfKeyFrames;
        AnimationCurve *curve=new AnimationCurve();
        for(int j=0;j<numOfKeyFrames;j++) {
            qreal time,value;
            in >> time >> value;
            curve->addKeyFrame(time,value);
        }
        curve->generateIndexes();
        animCurves.insert(curveName,curve);
    }
    if(animCurves.count()>0) {
        anim=new Animation(animCurves,name);
    } else {
        anim=new Animation();
        anim->setName(name);
    }
    BrainiacGlobals::AnimationType animType;
    in >> (quint32&) animType;
    bool isLoopedAnimation;
    in >> isLoopedAnimation;
    bool isRetriggerable;
    in >> isRetriggerable;
    anim->setAnimationType(animType);
    anim->setIsLoopedAnimation(isLoopedAnimation);
    anim->setIsRetriggerable(isRetriggerable);

//    QHashIterator<QString, LatchCurve*> iLatch(m_latchCurves) ;
//    out << m_latchCurves.count();
//    while(iLatch.hasNext()) {
//        iLatch.next();
//        out << iLatch.key();
//        LatchCurve *latch=iLatch.value();
//        out << latch->latches().count();
//        foreach(QVector2D l, latch->latches()) {
//            out << l;
//        }
//    }
    int numOfLatchCurves;
    in >> numOfLatchCurves;
    for( int i=0; i<numOfLatchCurves; i++ ) {
        QString latchName;
        in >> latchName;
        int numOfLatches;
        in >> numOfLatches;
        //LatchCurve *curve=new LatchCurve();
        anim->addLatchCurve(latchName);
        for( int i=0;i<numOfLatches;i++) {
            qreal time,length;
            in >> time;
            in >> length;
            anim->addLatch(latchName,time,length);
        }
    }

    int numOfTransitionKeyFrames;
    in >> numOfTransitionKeyFrames;
    if(numOfTransitionKeyFrames>0) {
        AnimationCurve transitionCurve;
        for(int i=0; i< numOfTransitionKeyFrames; i++) {
            qreal time,length;
            in >> time;
            in >> length;
            transitionCurve.addKeyFrame(time,length);
        }
        anim->setTransitionCurve(transitionCurve);
    }

    anim->setFileName(fileName);
    //qDebug( )  << "Magic number <<" << magicNumber << version << name <<  numOfCurves;
    return anim;
}

bool Animation::saveAnimation()
{
    return saveAnimation(m_fileName);
}

bool Animation::saveAnimation(QString &fileName)
{
    QReadLocker rLocker(&m_rwLock);
    Q_UNUSED(rLocker);
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)){
        qWarning() << __PRETTY_FUNCTION__ << "Could not open file "<< fileName << "for writing";
        return false;
    }
    QDataStream out(&file);

    // Write a header with a "magic number" and a version
    out << BrainiacGlobals::animationFileFormatMagicNumber;
    out << (qint32)BrainiacGlobals::animationFileFormatVersion;

    out.setVersion(QDataStream::Qt_4_8);
    QHashIterator<QString, AnimationCurve*> i(m_curves) ;
    out << m_name;
    out << m_curves.count();
    while(i.hasNext()) {
        i.next();
        out << i.key();
        AnimationCurve* curve=i.value();
        out << curve->keyFrames().count();
        for(int keyFrameNo=0; keyFrameNo < curve->keyFrames().count();keyFrameNo++)
        {
            QVector3D kf=curve->keyFrames().at(keyFrameNo);
            out << kf.x() << kf.y();
        }
    }
    m_fileName=fileName;
    out << (quint32)m_animType;
    out << m_isLoopedAnimation;
    out << m_isRetriggerable;
    QHashIterator<QString, LatchCurve*> iLatch(m_latchCurves) ;
    out << m_latchCurves.count();
    while(iLatch.hasNext()) {
        iLatch.next();
        out << iLatch.key(); // name of latch
        LatchCurve *latch=iLatch.value();
        out << latch->latches().count();
        foreach(QVector2D l, latch->latches()) {
            out << l;
        }
    }

    // Transition curve
    if(m_transitionCurve) {
        out << m_transitionCurve->keyFrames().count();
        for(int keyFrameNo=0; keyFrameNo < m_transitionCurve->keyFrames().count();keyFrameNo++)
        {
            QVector3D kf=m_transitionCurve->keyFrames().at(keyFrameNo);
            out << kf.x() << kf.y();
        }
    } else {
        out << 0;
    }



    return true;
}

void Animation::setName(QString name)
{
    m_name=name;
    m_channelPhaseOffsetName=m_name%BrainiacGlobals::ChannelPhaseOffsetSuffix;
    m_channelRunningName=m_name%BrainiacGlobals::ChannelRunningSuffix;
}

void Animation::setTransitionCurve(AnimationCurve transitionCurve) {
    QWriteLocker wLocker(&m_rwLock);
    if(m_transitionCurve) {
        delete m_transitionCurve;
    }
    m_transitionCurve=new AnimationCurve(&transitionCurve);

}

Animation::~Animation()
{
    deleteCurves();
}
