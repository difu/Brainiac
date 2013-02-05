#include "bodysegmentchannelhandler.h"
#include "core/agent/body/bodysegment.h"

BodySegmentChannelHandler::BodySegmentChannelHandler(BodySegment *bodySegment, QObject *parent) :
    QObject(parent)
{
    m_bodySegment=bodySegment;
}
