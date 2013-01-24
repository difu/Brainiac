// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

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

#ifndef SEGMENTSHAPE_H
#define SEGMENTSHAPE_H

#include "segment.h"

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Transform>

/**
 * @brief This class describes as segment and also holds the geometry that is shared between all agents of an AgentManager
 *
 * @class SegmentShape segmentshape.h "core/agent/body/segmentshape.h"
 */
class SegmentShape : public Segment
{
public:
/**
 * @brief Copy Constructor
 *
 * @fn SegmentShape
 * @param other
 */
    SegmentShape(const Segment &other);
    /**
     * @brief Destructor
     *
     * @fn ~SegmentShape
     */
    virtual ~SegmentShape();

protected:
    osg::ref_ptr<osg::Geode> m_geode; /**< the geometry node that holds the ShapeDrawable */
    osg::ref_ptr<osg::ShapeDrawable> m_shapeDrawable; /**< the node that holds the geometyr */
    osg::ref_ptr<osg::Transform> m_transformNode; /**< the transformation node of the segment. This is the node that is shared between all agent instances @sa AgentManager */
};

#endif // SEGMENTSHAPE_H
