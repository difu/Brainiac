#include "fuzzychannel.h"
#include "core/agent/channel.h"
#include "core/agent/agent.h"
#include "core/agent/brain/brain.h"
#include "core/brainiaclogger.h"

FuzzyChannel::FuzzyChannel(LogicType logicType, quint32 id, Brain *brain, const QString& name, const QString& channelName, qreal min, qreal max):
    FuzzyBase(logicType, brain, id, name, min, max),
    m_channel(0)
{
    setChannelName(channelName);
}

void FuzzyChannel::channelDelete()
{
    m_channel=0;
}
QString FuzzyChannel::getChannelName() const
{
    QString tmpName;
    switch(m_logicType) {
    case INPUT:
        tmpName=m_brain->getAgent()->getInputChannelName(m_channel);
        break;
    case OUTPUT:
        tmpName=m_brain->getAgent()->getOutputChannelName(m_channel);
        break;
    default:
        qCritical() << __PRETTY_FUNCTION__ << "Unknown FuzzType " << m_logicType;
    }
    if(tmpName.size()==0) {
        qCDebug(bChannel) << __PRETTY_FUNCTION__ << "Bad, did not find channel in agent list, returning cached value " << m_channelName  << "agentName " << m_brain->getAgent()->objectName();
        return m_channelName;
    } else {
        return tmpName;
    }
//    qCDebug(bChannel) << __PRETTY_FUNCTION__ << "Channel with fuzz type " << m_logicType <<" not found.";
//    return QString();
}

void FuzzyChannel::setChannelName(const QString& channelName)
{
    m_channelName=channelName;
    Channel *newChannel=0;
    switch(m_logicType) {
    case INPUT:
        newChannel=m_brain->getAgent()->getInputChannel(channelName);
        break;
    case OUTPUT:
        newChannel=m_brain->getAgent()->getOutputChannel(channelName);
        break;
    default:
        qCritical() << __PRETTY_FUNCTION__ << "Unknown FuzzType " << m_logicType;
    }
    if(!newChannel) {
        qCDebug(bChannel) << __PRETTY_FUNCTION__ << "Channel with fuzz type " << m_logicType <<", id "<< m_id << " and channel name " << channelName << " does not exist";
    }
    // if nothing changed, return
    if(newChannel==m_channel) {
        return;
    }

    if(m_channel) {
        disconnect(m_channel,0,this,0);
    }

    m_channel=newChannel;

    if(m_channel) {
        connect(m_channel,SIGNAL(destroyed()),SLOT(channelDelete()),Qt::DirectConnection);
        setResult(m_channel->getOldValue());
    }
}
