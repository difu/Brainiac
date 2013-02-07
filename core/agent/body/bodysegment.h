#ifndef BODYSEGMENT_H
#define BODYSEGMENT_H

#include <osg/MatrixTransform>
#include "core/agent/body/segmentshape.h"

class BodySegmentChannelHandler;
class Body;

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
    BodySegment(Body *body,SegmentShape *segmentShape);
    void computeMatrix();
    quint32 getId() const { return m_segmentShape->getId(); }
    quint32 getParentId() const { return m_segmentShape->getParentId(); }
protected :
    BodySegmentChannelHandler *m_channelHandler;
    Body *m_body; //!< The Body this BodySegment belongs to
    virtual ~BodySegment();
    SegmentShape *m_segmentShape;
    bool m_matrixDirty;
};

#endif // BODYSEGMENT_H
