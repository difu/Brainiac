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


#include "animationcurve.h"
#include <QDebug>
#include <QtGlobal>

AnimationCurve::AnimationCurve()
{
}

AnimationCurve::AnimationCurve(AnimationCurve *curve)
{
    foreach(QVector2D *vec,curve->keyFrames()) {
        addKeyFrame(vec->x(),vec->y());
    }
}

void AnimationCurve::addKeyFrame(qreal time, qreal value)
{
    // check, if we have already a keyframe at given time.
    // if so, change its value
    for(int i=0; i<m_keyFrames.count();i++) {
        if(qFuzzyCompare((qreal)m_keyFrames.at(i)->x(),time)) {
            m_keyFrames[i]->setY(value);
            return;
        }
    }

    if(m_keyFrames.count()==0) {
        m_keyFrames.append(new QVector2D(time,value));
        return;
    } else {
        for(int i=0;i<m_keyFrames.count();i++) {
            if(m_keyFrames.at(i)->x()>time) {
                m_keyFrames.insert(i,new QVector2D(time,value));
                return;
            }
        }
    }
    m_keyFrames.append(new QVector2D(time,value));

}

void AnimationCurve::addKeyFrame(QVector2D kf)
{
    addKeyFrame(kf.x(),kf.y());
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
            if(m_keyFrames.at(i)->x()>time) {
                QVector2D *kf=m_keyFrames.at(i);
                delete kf;
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
            if(m_keyFrames.first()->x()<time) {
                QVector2D *kf=m_keyFrames.takeFirst();
                delete kf;
            }
        }
    }
}

qreal AnimationCurve::getMaxValue() const
{
    qreal maxValue=0.0f;
    if(!m_keyFrames.isEmpty()) {
        maxValue=m_keyFrames.first()->y();
        foreach(QVector2D *kf, m_keyFrames) {
            if(kf->y()>maxValue) {
                maxValue=kf->y();
            }
        }
    }
    return maxValue;
}

qreal AnimationCurve::getMinValue() const
{
    qreal minValue=0.0f;
    if(!m_keyFrames.isEmpty()) {
        minValue=m_keyFrames.first()->y();
        foreach(QVector2D *kf, m_keyFrames) {
            if(kf->y()<minValue) {
                minValue=kf->y();
            }
        }
    }
    return minValue;
}

qreal AnimationCurve::getValue(qreal time) const
{
    qreal retVal=0;
    int numKeyFrames=m_keyFrames.size();
    if(numKeyFrames==0)
        return retVal;
    else {
        if(numKeyFrames==1 || m_keyFrames.at(0)->x()>=time) {
            return m_keyFrames.first()->y();
        } else {
            for(int i=1;i<numKeyFrames;++i) {
//                if(qFuzzyCompare(time,(qreal)m_keyFrames.at(i-1)->x())) {
//                    return m_keyFrames.at(i-1)->y();
//                }
                QVector2D *lowerKf=m_keyFrames.at(i-1);
                QVector2D *upperKf=m_keyFrames.at(i);
                qreal lower=lowerKf->x();
                qreal upper=upperKf->x();
                if(time>=lower && time<upper) {
                    qreal dist=upper-lower;
                    qreal factor=(time-lower)/dist;
                    return lowerKf->y()+(upperKf->y()-lowerKf->y())*factor;
                }
            }
            return m_keyFrames.last()->y();
        }
    }
    return 0.0;
}

void AnimationCurve::dPrintKeyFrames(quint32 start=0, quint32 end=1000) const
{
    qDebug() << __PRETTY_FUNCTION__;
    for(quint32 i=start;i<m_keyFrames.count()&& i < end;++i) {
        qDebug() <<"Keyframe " << i << " Time: " << m_keyFrames.at(i)->x() << "Value" << m_keyFrames.at(i)->y();
    }
}

AnimationCurve::~AnimationCurve()
{
//    foreach(QVector2D *kf, m_keyFrames) {
//        delete kf;
//    }

//    m_keyFrames.clear();
//    while(!m_keyFrames.isEmpty()) {
//        delete m_keyFrames.takeLast();
//    }
}
