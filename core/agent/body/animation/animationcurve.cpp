#include "animationcurve.h"
#include <QDebug>

AnimationCurve::AnimationCurve()
{
}

AnimationCurve::AnimationCurve(AnimationCurve *curve)
{
    foreach(QVector2D vec,curve->keyFrames()) {
        addKeyFrame(vec.x(),vec.y());
    }
}

void AnimationCurve::addKeyFrame(qreal time, qreal value)
{
    // check, if we have already a keyframe at given time.
    // if so, change its value
    for(int i=0; i<m_keyFrames.count();i++) {
        if(qFuzzyCompare(m_keyFrames.at(i).x(),time)) {
            m_keyFrames[i].setY(value);
            return;
        }
    }

    if(m_keyFrames.count()==0) {
        m_keyFrames.append(QVector2D(time,value));
        return;
    } else {
        for(int i=0;i<m_keyFrames.count();i++) {
            if(m_keyFrames.at(i).x()>time) {
                m_keyFrames.insert(i,QVector2D(time,value));
                return;
            }
        }
    }
    m_keyFrames.append(QVector2D(time,value));

}

AnimationCurve* AnimationCurve::clone()
{
    AnimationCurve *c=new AnimationCurve(this);
    return c;
}

void AnimationCurve::deleteAfterTime(qreal time)
{
    if(!m_keyFrames.isEmpty()) {
        for(int i=m_keyFrames.count()-1;i>=0;i--) {
            if(m_keyFrames.at(i).x()>time) {
                m_keyFrames.removeAt(i);
            }
        }
    }
}

void AnimationCurve::deleteBeforeTime(qreal time)
{
    while(true) {
        if(m_keyFrames.isEmpty()) {
            return;
        } else {
            if(m_keyFrames.first().x()<time)
                m_keyFrames.takeFirst();
        }
    }
}

qreal AnimationCurve::getValue(qreal time) const
{
    qreal retVal=0;
    if(m_keyFrames.isEmpty())
        return retVal;
    else {
        if(m_keyFrames.count()==1 || m_keyFrames.at(0).x()>=time) {
            return m_keyFrames.first().y();
        } else {
            for(int i=1;i<m_keyFrames.size();++i) {
                if(qFuzzyCompare(time,m_keyFrames.at(i-1).x())) {
                    return m_keyFrames.at(i-1).y();
                }
                qreal lower=m_keyFrames.at(i-1).x();
                qreal upper=m_keyFrames.at(i).x();
                if(time>lower && time<upper) {
                    qreal dist=upper-lower;
                    qreal factor=(time-lower)/dist;
                    return m_keyFrames.at(i-1).y()+(m_keyFrames.at(i).y()-m_keyFrames.at(i-1).y())*factor;
                }
            }
            return m_keyFrames.last().y();
        }
    }
    return 0.0f;
}

void AnimationCurve::dPrintKeyFrames(quint32 start=0, quint32 end=1000) const
{
    qDebug() << __PRETTY_FUNCTION__;
    for(int i=start;i<m_keyFrames.count()&& i < end;++i) {
        qDebug() <<"Keyframe " << i << " Time: " << m_keyFrames.at(i).x() << "Value" << m_keyFrames.at(i).y();
    }
}

AnimationCurve::~AnimationCurve()
{
    m_keyFrames.clear();
}
