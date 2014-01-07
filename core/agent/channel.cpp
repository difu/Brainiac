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


/** \brief Constructor
        Init the channel with a max, min and a initial value
**/
Channel::Channel(qreal min, qreal max, qreal value)
{
    m_max=max;
    m_min=min;
    m_oldValue=m_value=qBound(m_min,value,m_max);
}

/** \brief set channels old value to actual value
**/
void Channel::advance()
{
    if(m_oldValue!=m_value) {
        m_oldValue=m_value;
        emit oldValueChanged(m_oldValue);
    }
}

/** \brief changes the value of this channel
        This slot is invoked if this channel has a parent
        @sa Channel::setInherited()
**/
void Channel::changeValue(qreal value)
{
    setValue(value);
}

/** \brief returns the max value of this channel
          note that this value may change from the min value of a fuzzy node!
          @sa FuzzyBase::getMaxValue()
**/
qreal Channel::getMaxValue() const
{
    return m_max;
}

/** \brief returns the min value of this channel
        note that this value may change from the min value of a fuzzy node!
        @sa FuzzyBase::getMinValue()
**/
qreal Channel::getMinValue() const
{
    return m_min;
}

/** \brief @returns the range between min and max
**/
qreal Channel::getRange() const
{
    return qAbs(m_max-m_min);
}

/** \brief @returns the current value of this channel
**/
qreal Channel::getValue() const {
    return m_value;
}

qreal Channel::getValue(Agent *agent, const QString &channelName)
{
    Channel *c=agent->getOutputChannel(channelName);
    if(c)
        return c->getValue();
}

/** \brief @returns the old value of this channel
**/
qreal Channel::getOldValue() const {
    return m_oldValue;
}

/** \brief @returns true, if this channel´s value is inherited
**/
bool Channel::isInherited() const {
    return m_inherited;
}

/** \brief inits channel

                the channel is initialized with given value.
                Bounds are not changed!

**/
void Channel::init(qreal value)
{
    m_oldValue=m_value=value;
    emit valueChanged(value);
}

/** \brief makes this channel inherit its parent´s value

                a channel´s value is automatically changing its value when the parent´s value changes

                @param parent this channel´s parent
                @param inherited true, if this channel should inherit
                @todo rewrite that inherited is no more needed, to prevent unexpected behavior when parent changes and signal to previous parent is not disconnected

**/
void Channel::setInherited(Channel *parent, bool inherited)
{
    m_inherited=inherited;
    if(m_inherited)
        connect(parent,SIGNAL(valueChanged(qreal)),this,SLOT(changeValue(qreal)),Qt::DirectConnection);
    else
        disconnect(parent,SIGNAL(valueChanged(qreal)),this,SLOT(changeValue(qreal)));
}

/** \brief sets channel´s value

                set the channels value.
                the value is cropped to the channels limits

**/
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
