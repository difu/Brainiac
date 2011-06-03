#ifndef CHANNEL_H
#define CHANNEL_H

#include<QtCore>
#include<QObject>

class Channel: public QObject
{
    Q_OBJECT
public:
    Channel(qreal min=-std::numeric_limits<qreal>::max(), qreal max=std::numeric_limits<qreal>::max(), qreal value=0);
    void advance();
    void init(qreal value);
    bool isInherited();
    qreal getValue();
    void setValue(qreal value, bool isSpeed=false);
    void setInherited(Channel *parent, bool inherited);

public slots:
    void changeValue(qreal value);

protected:
    qreal m_value;
    bool m_inherited;
    qreal m_oldValue;
    qreal m_max;
    qreal m_min;
    QString m_name;
signals:
    void valueChanged(qreal value);
};

#endif // CHANNEL_H
