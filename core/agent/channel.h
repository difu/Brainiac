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


#ifndef CHANNEL_H
#define CHANNEL_H

#include<QtCore>
#include<QObject>
#include"core/brainiacglobals.h"

class Agent;

/** \brief  An agent´s channel

        A channel may be an input or output source for the agent´s brain
        @sa Brain
        @sa Agent

**/
class Channel: public QObject
{
    Q_OBJECT
public:
    Channel(qreal min=BrainiacGlobals::MINQREAL, qreal max=BrainiacGlobals::MAXQREAL, qreal value=0);
    void advance();
    qreal getMaxValue() const;
    qreal getMinValue() const;
    qreal getRange() const;
    void init(qreal value);
    bool isInherited() const;
    qreal getValue() const;
    static qreal getValue(Agent *agent, const QString& channelName);
    qreal getOldValue() const;
    void setMinValue( qreal min ) { m_min=min; }
    void setMaxValue( qreal max ) { m_max=max; }
    void setValue(qreal value, bool isSpeed=false);
    void setInherited(Channel *parent, bool inherited);

public slots:
    void changeValue(qreal value); //!< Slot to connect for channel inheritance  @sa Channel::setInherited()

protected:
    qreal m_value; //!< Current value of this channel
    bool m_inherited; //!< true, if this channel is inherited
    qreal m_oldValue; //!< the old value of the channel
    qreal m_max; //!< the max value
    qreal m_min; //!< the min value
    QString m_name; //!< this channel´s name
signals:
    void valueChanged(qreal value); //!< emitted if the value of this channel changes @sa Channel::setInherited()
    void oldValueChanged(qreal oldValue); //!< emitted if oldValue has changed after advance()
};

#endif // CHANNEL_H
