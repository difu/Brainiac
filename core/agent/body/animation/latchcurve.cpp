#include "latchcurve.h"

LatchCurve::LatchCurve()
{
}

LatchCurve::LatchCurve(const LatchCurve &curve)
{
    m_latches.clear();
    foreach(QVector2D vec,curve.latches())
    {
        m_latches.append(vec);
    }
}

void LatchCurve::addLatch(qreal time, qreal duration)
{
    m_latches.append(QVector2D(time,duration));
}

void LatchCurve::deleteLatch(qreal time)
{
    for(int i=0;i<m_latches.count();i++) {
        qreal start=m_latches.at(i).x();
        qreal end=m_latches.at(i).y()+start;
        if(time>=start && time<=end) {
            m_latches.removeAt(i);
            return;
        }
    }
}

bool LatchCurve::isLatch(qreal time) const
{
    for(int i=0;i<m_latches.count();i++) {
        qreal start=m_latches.at(i).x();
        qreal end=m_latches.at(i).y()+start;
        if(time>=start && time<=end) {
            return true;
        }
    }
    return false;
}

QList<QVector2D> LatchCurve::latches() const
{
 return m_latches;
}
