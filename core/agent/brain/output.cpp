#include "output.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/fuzzydefuzz.h"
#include "core/agent/agent.h"
#include "core/agent/channel.h"

Output::Output( quint32 id, Brain *brain, QString name, QString channel, qreal min, qreal max) : FuzzyBase(FuzzyBase::OUTPUT, brain, id, name, min, max)
{
    setChannelName(channel);
    m_defuzzMode=AVERAGE;
}

void Output::calculate()
{
    bool defuzzFound=false;
    qreal avgResult=0;
    quint32 numOfDefuzzes=0;
    qreal maxResult=0;
    qreal maxDefuzzValue;
    foreach(Parent par, m_parents) {
        if(par.parent->getType()==DEFUZZ) {
            defuzzFound=true;
            FuzzyDefuzz *defuzz=(FuzzyDefuzz *)par.parent;
            qreal result=defuzz->getResult(par.inverted);
            avgResult+=result*BrainiacGlobals::deNorm(m_minValue,m_maxValue,defuzz->getDefuzzVal());
            if(result>maxResult) {
                maxDefuzzValue=defuzz->getDefuzzVal();
                maxResult=result;
            }
            //qDebug() << "result" << result << avgResult <<BrainiacGlobals::deNorm( m_minValue,m_maxValue,defuzz->getDefuzzVal()) << defuzz->getDefuzzVal();
            numOfDefuzzes++;
        }
    }
    if(defuzzFound) {
        switch(m_defuzzMode) {
        case AVERAGE:
            setResult(avgResult);
            break;
        case MAX:
            setResult(BrainiacGlobals::deNorm(m_minValue,m_maxValue,maxDefuzzValue)*maxResult);
            break;
        }
    } else {
        if(m_parents.count()>0) {
            Parent par=m_parents.at(0);
            setResult(par.parent->getResult(par.inverted));// Only take the first parent
        }
    }
}

/** \brief returns the channel name

                Outputs may (and mostly they are) connected to an output channel. This function provides access

        \return the name of the channel associated with this output
**/
QString Output::getChannelName()
{
    return m_channelName;
}

Output::DefuzzMode Output::getDefuzzMode()
{
    return m_defuzzMode;
}

void Output::setChannelName(QString channel)
{
    m_channelName=channel;
    m_channel=m_brain->getAgent()->getOutputChannel(m_channelName);
//    m_maxValue=m_channel->getMax();
//    m_minValue=m_channel->getMin();
    if(m_result)
        setResult(m_channel->getValue());
}

void Output::setDefuzzMode(DefuzzMode mode)
{
    m_defuzzMode=mode;
}

/** \brief sets the result of this output node

                sets the result and if this node is associated with a channel it writes it in.

        \param  result the result
        @returns true if result was changed
**/
bool Output::setResult(qreal result,bool emitChange)
{
    bool changed=FuzzyBase::setResult(result,emitChange);
    if(m_channel) {
        m_channel->setValue(m_result);
    } else {
        //qDebug() << __PRETTY_FUNCTION__ << "channel " << m_channelName<< "does not exist!";
    }
    return changed;
}
