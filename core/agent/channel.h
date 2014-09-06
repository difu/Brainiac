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

    /**
     * @brief Behavior, if channel does not exist
     *
     * @enum ChannelNotExistOptions
     */
    enum ChannelNotExistOptions { NONE, /**< Ignore */
                                  NONE_WARN, /**< Ignore but write warning to stderr*/
                                  CREATE_IF_NOT_EXISTS /**< Create a new channel */
                                };

    /** \brief Constructor
            Init the channel with a max, min and a default value, that is also set as current and old value
    **/
    Channel(Agent *agent, qreal min=BrainiacGlobals::MINQREAL, qreal max=BrainiacGlobals::MAXQREAL, qreal value=0.0);

    /** \brief set channels old value to actual value
    **/
    void advance();

    static qreal getInputValue(Agent *agent, const QString& channelName);

    /** \brief returns the max value of this channel
              note that this value may change from the min value of a fuzzy node!
              @sa FuzzyBase::getMaxValue()
    **/
    qreal getMaxValue() const;

    /** \brief returns the min value of this channel
            note that this value may change from the min value of a fuzzy node!
            @sa FuzzyBase::getMinValue()
    **/
    qreal getMinValue() const;
    static qreal getOutputValue(Agent *agent, const QString& channelName);

    /** \brief @returns the range between min and max
    **/
    qreal getRange() const;

    /** \brief sets the default value of this channel channel

                    the channel is initialized with given value.
                    Bounds are not changed!

    **/
    void setDefault(qreal value);
    bool isInherited() const;

    /** \brief @returns the current value of this channel
    **/
    qreal getValue() const;

    /** \brief @returns the old value of this channel
    **/
    qreal getOldValue() const;

    /** \brief resets the channel
     * the default value is applied both for value and old value
     * @emits valueChanged
    **/
    void reset();
    void setMinValue( qreal min ) { m_min=min; }
    void setMaxValue( qreal max ) { m_max=max; }


    /**
     * @brief sets the value of an input channel
     *
     * @fn setInputValue
     * @param agent pointer to agent
     * @param channelName the name of the input channel
     * @param value the vakue to be set
     * @param options Behavior, if channel does not exist
     * @sa SetChannelOptions
     */
    static void setInputValue(Agent *agent, const QString& channelName, qreal value, ChannelNotExistOptions options=Channel::NONE);

    /** \brief sets channel´s value

                    set the channels value.
                    the value is cropped to the channels limits

    **/

    /**
     * @brief sets the value of an oupput channel
     *
     * @fn setOutputValue
     * @param agent pointer to agent
     * @param channelName the name of the output channel
     * @param value the vakue to be set
     * @param options Behavior, if channel does not exist
     * @sa SetChannelOptions
     */
    static void setOutputValue(Agent *agent, const QString& channelName, qreal value, ChannelNotExistOptions options=Channel::NONE);

    /** \brief sets channel´s value

                    set the channels value.
                    the value is cropped to the channels limits

    **/
    void setValue(qreal value, bool isSpeed=false);

    /** \brief makes this channel inherit its parent´s value

                    a channel´s value is automatically changing its value when the parent´s value changes

                    @param parent this channel´s parent
                    @param inherited true, if this channel should inherit
                    @todo rewrite that inherited is no more needed, to prevent unexpected behavior when parent changes and signal to previous parent is not disconnected

    **/
    void setInherited(Channel *parent, bool inherited);


public slots:
    /** \brief changes the value of this channel
            This slot is invoked if this channel has a parent
            @sa Channel::setInherited()
    **/
    void changeValue(qreal value); //!< Slot to connect for channel inheritance  @sa Channel::setInherited()

protected:
    qreal m_defaultValue; //!< the default value of this channel
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
