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


#ifndef TRANSITIONCURVE_H
#define TRANSITIONCURVE_H

#include<QList>
#include<QVector2D>

/**
 * @brief manages a transition curve
 *
 * @class TransitionCurve transitioncurve.h "core/agent/body/animation/transitioncurve.h"
 */
class LatchCurve
{
public:

/**
 * @brief
 *
 * @fn LatchCurve
 */
    LatchCurve();

 /**
 * @brief
 *
 * @fn LatchCurve
 * @param curve
 */
    LatchCurve(const LatchCurve &curve);
    /**
     * @brief
     *
     * @fn addLatch
     * @param time
     * @param duration
     */
    void addLatch(qreal time, qreal duration);

    /**
     * @brief returns true, if the animation has a latch at given time
     *
     * if the animation has a latch, it should not be left
     *
     * @fn isLatch
     * @param time
     * @return bool
     */
    bool isLatch(qreal time) const;

    /**
     * @brief deletes a latch at given time
     *
     * @fn deleteLatch
     * @param time
     */
    void deleteLatch(qreal time);
    QList<QVector2D> latches() const;
protected:
    QList<QVector2D> m_latches; /**< all latches */
};

#endif // TRANSITIONCURVE_H
