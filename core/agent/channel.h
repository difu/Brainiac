#ifndef CHANNEL_H
#define CHANNEL_H

#include<QtCore>
#include<QObject>
#include"core/brainiacglobals.h"

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
    qreal getMaxValue();
    qreal getMinValue();
    qreal getRange();
    void init(qreal value);
    bool isInherited();
    qreal getValue();
    qreal getOldValue();
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
    void valueChanged(qreal value); //!< emitted when the value of this channel changes @sa Channel::setInherited()
};

#endif // CHANNEL_H
