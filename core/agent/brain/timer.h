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


#ifndef TIMER_H
#define TIMER_H

#include "core/agent/brain/fuzzybase.h"
#include <QtCore>
/** \brief  Timer fuzz node

        Creates a timer fuzz node for a given rate and behaviour

**/
class Timer : public FuzzyBase
{
public:
    enum TimerMode {ALWAYS, IFSTOPPED};
    Timer(quint32 id, Brain *brain, QString name, qreal rate, TimerMode mode);
    void advance();
    TimerMode getMode();
    qreal getRate();
    void setMode(TimerMode mode);
    void setRate(qreal rate);
    void calculate();

protected:
    TimerMode m_mode; //!< the mode of this timer
    qreal m_rate; //!< the rate of this timer
    bool m_running; //!< true, if this timer is currently running
    bool m_shouldRun;
    quint32 m_startFrame; //!< start frame no. of current loop
    quint32 m_frameDist; //!< time in frames for given rate
};

#endif // TIMER_H
