#include "channel.h"

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
    m_oldValue=m_value;
}

/** \brief inits channel
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
qreal Channel::getMaxValue()
{
    return m_max;
}

/** \brief returns the min value of this channel
        note that this value may change from the min value of a fuzzy node!
        @sa FuzzyBase::getMinValue()
**/
qreal Channel::getMinValue()
{
    return m_min;
}

/** \brief @returns the range between min and max
**/
qreal Channel::getRange()
{
    return qAbs(m_max-m_min);
}

/** \brief @returns the current value of this channel
**/
qreal Channel::getValue() {
    return m_value;
}

/** \brief @returns the old value of this channel
**/
qreal Channel::getOldValue() {
    return m_oldValue;
}

/** \brief @returns true, if this channel´s value is inherited
**/
bool Channel::isInherited() {
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
