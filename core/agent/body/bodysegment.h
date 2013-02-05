#ifndef BODYSEGMENT_H
#define BODYSEGMENT_H

#include <osg/MatrixTransform>
#include "core/agent/body/segmentshape.h"

class BodySegmentChannelHandler;

/**
 * @brief The BodySegment class
 *
 * Represents a segment of a body.
 */
class BodySegment: public osg::MatrixTransform
{
public:
    /**
     * @brief BodySegment Constructor
     * @param segmentShape the SegmentShape this BodySegment is based on
     */
    BodySegment(SegmentShape *segmentShape);
    void computeMatrix();
    quint32 getId() const { return m_segmentShape->getId(); }
protected :
    BodySegmentChannelHandler *m_channelHandler;
    virtual ~BodySegment();
    SegmentShape *m_segmentShape;
    bool m_matrixDirty;
};

#endif // BODYSEGMENT_H
