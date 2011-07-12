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
    m_rate=rate;
    quint32 fps=m_brain->getAgent()->getScene()->getSimulation()->getFps();
    m_frameDist=(quint32) m_rate*fps;
}
