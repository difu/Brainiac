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
