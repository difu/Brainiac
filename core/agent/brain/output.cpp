#include "output.h"
#include "core/agent/brain/brain.h"
#include "core/agent/agent.h"
#include "core/agent/channel.h"

Output::Output( quint32 id, Brain *brain, QString name, QString channel, quint32 editorX, quint32 editorY) : FuzzyBase(FuzzyBase::OUTPUT, brain, id, name, editorX, editorY)
{
    setChannelName(channel);
}

void Output::calculate()
{

}

/** \brief returns the channel name

                Outputs may (and mostly they are) connected to an output channel. This function provides access

        \return the name of the channel associated with this output
**/
QString Output::getChannelName()
{
    return m_channelName;
}

void Output::setChannelName(QString channel)
{
    m_channelName=channel;
    m_channel=m_brain->getAgent()->getOutputChannel(m_channelName);
    setResult(m_channel->getValue());
    m_maxValue=m_channel->getMax();
    m_minValue=m_channel->getMin();
}

/** \brief sets the result of this output node

                sets the result and if this node is associated with a channel it writes it in.

        \param  result the result
**/
void Output::setResult(qreal result)
{
    m_result=result;
    if(m_channel) {
        m_channel->setValue(result);
    } else {
        qDebug() << __PRETTY_FUNCTION__ << "channel " << m_channelName<< "does not exist!";
    }
}
