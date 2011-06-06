#include "input.h"
#include "core/agent/brain/brain.h"
#include "core/agent/agent.h"
#include "core/agent/channel.h"

Input::Input( quint32 id, Brain *brain, QString name, QString channel) : FuzzyBase(FuzzyBase::INPUT, brain, id, name)
{
    setChannelName(channel);
}

void Input::calculate()
{

}

/** \brief returns the channel name

                Inputs may (and mostly they are) connected to an input channel. This function provides access
                @sa output

        \return the name of the channel associated with this input
**/
QString Input::getChannelName()
{
    return m_channelName;
}

void Input::setChannelName(QString channel)
{
    m_channelName=channel;
    m_channel=m_brain->getAgent()->getInputChannel(m_channelName);
    setResult(m_channel->getValue());
    m_maxValue=m_channel->getMax();
    m_minValue=m_channel->getMin();
}

/** \brief sets the result of this input node

                sets the result

        \param  result the result
**/
void Input::setResult(qreal result)
{
    m_result=result;
    if(m_channel) {
        //m_channel->setValue(result);
    } else {
        qDebug() << __PRETTY_FUNCTION__ << "channel " << m_channelName<< "does not exist!";
    }
}
