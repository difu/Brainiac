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


#include "channel.h"
#include "core/agent/agent.h"


Channel::Channel(qreal min, qreal max, qreal value)
{
    m_max=max;
    m_min=min;
    m_oldValue=m_value=qBound(m_min,value,m_max);
    m_defaultValue=m_value;
}

void Channel::advance()
{
    if(m_oldValue!=m_value) {
        m_oldValue=m_value;
        emit oldValueChanged(m_oldValue);
    }
}

void Channel::changeValue(qreal value)
{
    setValue(value);
}

qreal Channel::getInputValue(Agent *agent, const QString &channelName)
{
    Channel *c=agent->getInputChannel(channelName);
    if(c)
        return c->getValue();
    return 0.0;
}

qreal Channel::getMaxValue() const
{
    return m_max;
}

qreal Channel::getMinValue() const
{
    return m_min;
}

qreal Channel::getRange() const
{
    return qAbs(m_max-m_min);
}

qreal Channel::getValue() const {
    return m_value;
}

qreal Channel::getOutputValue(Agent *agent, const QString &channelName)
{
    Channel *c=agent->getOutputChannel(channelName);
    if(c)
        return c->getValue();
    return 0.0;
}

qreal Channel::getOldValue() const {
    return m_oldValue;
}

bool Channel::isInherited() const {
    return m_inherited;
}

void Channel::reset()
{
    m_oldValue=m_value=m_defaultValue;
    emit valueChanged(m_defaultValue);
}

void Channel::setDefault(qreal value)
{
    value=qBound(m_min,value,m_max);
    m_oldValue=m_value=value;
    emit valueChanged(value);
}

void Channel::setInherited(Channel *parent, bool inherited)
{
    m_inherited=inherited;
    if(m_inherited)
        connect(parent,SIGNAL(valueChanged(qreal)),this,SLOT(changeValue(qreal)),Qt::DirectConnection);
    else
        disconnect(parent,SIGNAL(valueChanged(qreal)),this,SLOT(changeValue(qreal)));
}


void Channel::setValue(qreal value, bool isSpeed)
{
    qreal origValue=m_value;
    if(isSpeed) {
        m_value=m_oldValue+value;
    } else {
        m_value=value;
    }
    m_value=qBound(m_min,m_value,m_max);
    if(m_value!=origValue) {
        emit valueChanged(m_value);
    }
}
