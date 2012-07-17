#include "animation.h"
#include "core/agent/body/animation/animationcurve.h"
#include "core/brainiacglobals.h"

#include <QFile>
#include <QVector3D>
#include <QDebug>

Animation::Animation()
{
   calculateLength();
}

Animation::Animation( Animation *animation)
{
    QHashIterator<QString, AnimationCurve *> i(animation->curves()) ;

    while(i.hasNext()) {
        i.next();
        AnimationCurve *c=i.value();
        AnimationCurve *newCurve= new AnimationCurve(c);
        m_curves.insert(i.key(),newCurve);
    }
    calculateLength();
    m_name=animation->name();
    m_fileName=animation->fileName();
    m_isLoopedAnimation=animation->isLoopedAnimation();
}

Animation::Animation(QHash<QString, AnimationCurve *> curves, QString name="")
{
    m_name=name;
    m_curves=curves;
    m_isLoopedAnimation=true;
    calculateLength();
}

void Animation::calculateLength()
{
    m_length=0;
    foreach(AnimationCurve *c,m_curves) {
        qreal time=c->keyFrames().last().x();
        if( time > m_length) {
            m_length=time;
        }
    }
}

void Animation::deleteCurve(const QString &curveName)
{
    AnimationCurve *curve=m_curves.value(curveName,0);
    if(curve) {
        delete curve;
        m_curves.remove(curveName);
    }
}

void Animation::deleteCurves()
{
    foreach(AnimationCurve *curve,m_curves) {
        delete curve;
    }
    m_curves.clear();
}

qreal Animation::getLength(bool calculateNew)
{
    if(calculateNew) {
        calculateLength();
    }
    return m_length;
}

qreal Animation::getValue(const QString &curve, qreal time) const
{
    AnimationCurve *c=m_curves.value(curve,0);
    if(c) {
        return c->getValue(time);
    } else return 0.0f;

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
        animCurves.insert(curveName,curve);
    }
    if(animCurves.count()>0) {
        anim=new Animation(animCurves,name);
    } else {
        anim=new Animation();
        anim->name()=name;
    }
    anim->setFileName(fileName);
    qDebug( )  << "Magic number <<" << magicNumber << version << name <<  numOfCurves;
    return anim;
}

bool Animation::saveAnimation()
{
    return saveAnimation(m_fileName);
}

bool Animation::saveAnimation(QString &fileName)
{
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
    return true;
}

Animation::~Animation()
{
    deleteCurves();
}
