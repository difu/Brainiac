#ifndef BODYSEGMENTCHANNELHANDLER_H
#define BODYSEGMENTCHANNELHANDLER_H

#include <QObject>

class BodySegment;

class BodySegmentChannelHandler : public QObject
{
    Q_OBJECT
public:
    explicit BodySegmentChannelHandler(BodySegment *bodySegment, QObject *parent = 0);

signals:

public slots:

protected:
    BodySegment *m_bodySegment;

};

#endif // BODYSEGMENTCHANNELHANDLER_H
