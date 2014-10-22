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


#include "latchcurve.h"
#include <QDebug>

LatchCurve::LatchCurve()
{
}

LatchCurve::LatchCurve(const LatchCurve &curve)
{
    m_latches.clear();
    foreach(LatchType vec,curve.latches())
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
    bool deleted=true;
    while(deleted) {
        deleted=false;
        for(int i=0;i<m_latches.count();i++) {
            qreal start=m_latches.at(i).x();
            qreal end=m_latches.at(i).y()+start;
            if(time>=start && time<=end) {
                m_latches.removeAt(i);
                deleted=true;
                break;
            }
        }
    }
}

void LatchCurve::deleteLatch(int latchIndex)
{
    if(m_latches.count()>latchIndex) {
        m_latches.removeAt(latchIndex);
    } else {
        qWarning() << __PRETTY_FUNCTION__ << "Latch with id " << latchIndex << " does not exist";
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

QList<LatchCurve::LatchType> LatchCurve::latches() const
{
 return m_latches;
}
