#ifndef CHANNEL_H
#define CHANNEL_H

#include<QtCore>

class Channel
{
public:
    Channel(qreal min=-std::numeric_limits<qreal>::max(), qreal max=std::numeric_limits<qreal>::max(), qreal value=0);
    void advance();
    void init(qreal value);
    qreal getValue();
    void setValue(qreal value, bool isSpeed=false);

protected:
    qreal m_value;
    qreal m_oldValue;
    qreal m_max;
    qreal m_min;
    QString m_name;
};

#endif // CHANNEL_H
