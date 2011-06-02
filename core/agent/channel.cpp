#include "channel.h"

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

void Channel::changeValue(qreal value)
{
    setValue(value);
}

qreal Channel::getValue() {
    return m_value;
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
