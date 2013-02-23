// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2013  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef BODYSEGMENT_H
#define BODYSEGMENT_H

#include <osg/MatrixTransform>
#include "core/agent/body/segmentshape.h"

class BodySegmentSignalHandler;
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
    BodySegmentSignalHandler *m_channelHandler;
    Body *m_body; //!< The Body this BodySegment belongs to
    virtual ~BodySegment();
    SegmentShape *m_segmentShape;
    bool m_matrixDirty;
};

#endif // BODYSEGMENT_H
