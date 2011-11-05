#include "input.h"
#include "core/agent/brain/brain.h"
#include "core/agent/agent.h"
#include "core/agent/channel.h"

Input::Input( quint32 id, Brain *brain, QString name, QString channel, qreal min, qreal max) : FuzzyBase(FuzzyBase::INPUT, brain, id, name, min, max)
{
    setChannelName(channel);
}

void Input::calculate()
{
    if(m_channel) {
        setResult(m_channel->getValue());
    } else {
        //qDebug() << __PRETTY_FUNCTION__ << "channel " << m_channelName<< "does not exist!";
    }
}

QString Input::getChannelName()
{
    return m_channelName;
}

void Input::setChannelName(QString channel)
{
    if(     QString::compare(channel,BrainiacGlobals::ChannelName_Sound_a,Qt::CaseInsensitive)==0 ||
            QString::compare(channel,BrainiacGlobals::ChannelName_Sound_d,Qt::CaseInsensitive)==0 ||
            QString::compare(channel,BrainiacGlobals::ChannelName_Sound_f,Qt::CaseInsensitive)==0 ||
            QString::compare(channel,BrainiacGlobals::ChannelName_Sound_ox,Qt::CaseInsensitive)==0 ||
            QString::compare(channel,BrainiacGlobals::ChannelName_Sound_x,Qt::CaseInsensitive)==0 ) {
        m_isSoundInput=true;
    }
    m_channelName=channel;
    m_channel=m_brain->getAgent()->getInputChannel(m_channelName);
    if(m_channel)
        setResult(m_channel->getValue());
}

