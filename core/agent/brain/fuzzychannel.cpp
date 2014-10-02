// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2014  Dirk Fuchs dirkfux@googlemail.com

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

#include "fuzzychannel.h"
#include "core/agent/channel.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/brainmanager.h"
#include "core/brainiaclogger.h"

FuzzyChannel::FuzzyChannel(LogicType logicType, quint32 id, Brain *brain, const QString& channelName, qreal min, qreal max):
    FuzzyBase(logicType, brain, id, min, max),
    m_channel(0)
{
    setChannelName(channelName);
}

void FuzzyChannel::channelDelete()
{
    m_channel=0;
}

void FuzzyChannel::setChannelName(const QString& channelName)
{
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
        BrainManager *bm=m_brain->getAgent()->getAgentManager()->getBrainManager();
        qCDebug(bChannel) << __PRETTY_FUNCTION__ << "Channel of fuzz "<< bm->getFuzzyName(m_id) <<" type " << m_logicType <<", id "<< m_id << " and channel name " << channelName << " does not exist";
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
