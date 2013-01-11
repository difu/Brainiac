#ifndef SEGMENTSHAPE_H
#define SEGMENTSHAPE_H

#include "segment.h"

class SegmentShape : public Segment
{
public:
    SegmentShape(const Segment &other);
    virtual ~SegmentShape();
};

#endif // SEGMENTSHAPE_H
