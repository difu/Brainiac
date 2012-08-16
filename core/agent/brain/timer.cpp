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


#include "timer.h"
#include "core/agent/brain/brain.h"
#include "core/agent/agent.h"
#include "core/scene.h"
#include "core/simulation.h"

Timer::Timer(quint32 id, Brain *brain, QString name, qreal rate, TimerMode mode):FuzzyBase(FuzzyBase::TIMER,brain,id,name,0,1),m_mode(mode)
{
    m_running=false;
    m_startFrame=0;
    setRate(rate);
}

void Timer::advance()
{
    if(m_parents.count()>0) {
        if(m_shouldRun && !m_running) {
            m_running=true;
            m_startFrame=m_brain->getAgent()->getScene()->getSimulation()->getCurrentFrame();
        }
        if(m_running) {
            quint32 currentFrame=m_brain->getAgent()->getScene()->getSimulation()->getCurrentFrame();
            quint32 mod=(currentFrame-m_startFrame)%m_frameDist;
            setResult(((qreal)mod/(qreal)(m_frameDist)));
            if((currentFrame-m_startFrame)==m_frameDist)
                setResult(1.0f);
            if((currentFrame-m_startFrame)>m_frameDist) {
                setResult(0.0f);
                m_running=false;
            }
        }
    }  else { // it is a timer without parent(s), it loops continously
        quint32 currentFrame=m_brain->getAgent()->getScene()->getSimulation()->getCurrentFrame();
        quint32 mod=currentFrame%m_frameDist;
        m_running=true;
        setResult(((qreal)mod/(qreal)(m_frameDist)));
    }
}

void Timer::calculate()
{
    if(m_parents.count()>0) {
        Parent par=m_parents.at(0);// Only take the first parent
        if(m_mode==IFSTOPPED) {
            if(par.parent->getResult(par.inverted)>0.5) {
                m_shouldRun=true;
                //m_startFrame=m_brain->getAgent()->getScene()->getSimulation()->getCurrentFrame();
            } else {
                m_shouldRun=false;
            }
        } else {

        }

    }

}

Timer::TimerMode Timer::getMode()
{
    return m_mode;
}

qreal Timer::getRate()
{
    return m_rate;
}

void Timer::setMode(TimerMode mode)
{
    m_mode=mode;
}

void Timer::setRate(qreal rate)
{
    if(rate<0) {
        rate=0;
    }
    m_rate=rate;
    quint32 fps=m_brain->getAgent()->getScene()->getSimulation()->getFps();
    m_frameDist=(quint32) m_rate*fps;
}
