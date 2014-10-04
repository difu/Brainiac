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


#include "input.h"
#include "core/agent/brain/brain.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/brain/brainmanager.h"
#include "core/agent/brain/fuzzyproperties.h"
#include "core/agent/channel.h"

Input::Input(quint32 id, Brain *brain, const QString &channel, qreal min, qreal max) :
    FuzzyChannel(FuzzyBase::INPUT, id, brain, channel,  min, max)
{
    setChannelName(channel);
}

void Input::calculate()
{
    if(m_channel) {
        setResult(m_channel->getOldValue());
    } else {
        qDebug() << __PRETTY_FUNCTION__ << "channel does not exist!";
    }
}

void Input::setChannelName(const QString& channel)
{
    if(m_brain->getAgent()->getAgentManager()->getBrainManager()->getFuzzyProperties(m_id)->isChannelSound()) {
        m_isSoundInput=true;
    } else {
        m_isSoundInput=false;
    }
    FuzzyChannel::setChannelName(channel);
}

