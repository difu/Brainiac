#include "animation.h"
#include "core/agent/body/animation/animationcurve.h"

Animation::Animation()
{
   calculateLength();
}

Animation::Animation(const Animation &animation)
{
    QHashIterator<QString, AnimationCurve *> i(animation.curves()) ;

    while(i.hasNext()) {
        i.next();
        AnimationCurve *newCurve= new AnimationCurve(*i.value());
        m_curves.insert(i.key(),newCurve);
    }
    calculateLength();
    m_name=animation.name();
    m_fileName=animation.relativeFileName();
    m_isLoopedAnimation=animation.isLoopedAnimation();
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

Animation::~Animation()
{
    foreach(AnimationCurve *c,m_curves) {
        delete c;
    }
    m_curves.clear();
}
