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
#include "core/brainiaclogger.h"


Channel::Channel(Agent *agent, qreal min, qreal max, qreal value):
    QObject( qobject_cast<QObject *> (agent))
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
    emit oldValueChanged(m_defaultValue);
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

void Channel::setInputValue(Agent *agent, const QString &channelName, qreal value, ChannelNotExistOptions options)
{
    Channel *inp=agent->getInputChannel(channelName);
    if(inp) {
        inp->setValue(value);
        return;
    }
    switch(options) {
    case Channel::NONE:
        return;
        break;
    case Channel::NONE_WARN:
        qCDebug(bChannel) << __PRETTY_FUNCTION__ << "Input Channel "<< channelName << " does not exist!";
        break;
    case Channel::CREATE_IF_NOT_EXISTS:
        qCWarning(bChannel) << __PRETTY_FUNCTION__ << "Create new input channel!";
        Channel *newInput=new Channel(agent);
        newInput->setValue(value);
        bool success=agent->addInputChannel(newInput,channelName);
        if(!success) {
            qCWarning(bChannel) << __PRETTY_FUNCTION__ << "Input Channel creation failed!";
            delete newInput;
        }
    }
}

void Channel::setOutputValue(Agent *agent, const QString &channelName, qreal value, ChannelNotExistOptions options)
{
    Channel *out=agent->getOutputChannel(channelName);
    if(out) {
        out->setValue(value);
        return;
    }
    switch(options) {
    case Channel::NONE:
        return;
        break;
    case Channel::NONE_WARN:
        qCWarning(bChannel) << __PRETTY_FUNCTION__ << "Output Channel "<< channelName << " does not exist!";
        break;
    case Channel::CREATE_IF_NOT_EXISTS:
        qCritical() << __PRETTY_FUNCTION__ << "TODO";
//        qCDebug(bChannel) << __PRETTY_FUNCTION__ << "Create new output channel!";
//        Channel *newOutput=new Channel(agent);
//        newOutput->setValue(value);
//        bool success=agent->getOrCreateOutputChannel(newOutput,channelName);
//        if(!success) {
//            qCWarning(bChannel) << __PRETTY_FUNCTION__ << "Output Channel creation failed!";
//            delete newOutput;
//        }
    }
}
