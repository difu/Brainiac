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
#include <osg/Geode>
#include <osg/Switch>
#include <osgFX/Outline>
#include "core/agent/body/segmentshape.h"

class BodySegmentSignalHandler;
class Body;
class Channel;

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
     * @param Body the Body this BodySegment belongs to
     * @param segmentShape the SegmentShape this BodySegment is based on
     */
    BodySegment(Body *body,SegmentShape *segmentShape);
    void computeRestMatrix();
    void computeMatrix();
    quint32 getId() const { return m_segmentShape->getId(); }
    quint32 getParentId() const { return m_segmentShape->getParentId(); }
    SegmentShape *getSegmentShape() const { return m_segmentShape; }

    /**
     * @brief switches the highlightning of this BodySegment
     * @param highlight true if this BodySegment should be highlighted, false otherwise
     */
    void highlight(bool highlight) { m_shouldHighlight=highlight; }

    /**
     * @brief call this function, if the rest matrix should be calculated next traverse
     */
    void setRestMatrixDirty() { m_restMatrixDirty=true; }

    /**
     * @brief shows this Segment s pivot coordinate cross
     * @param show true if the coordinate cross should be shown, false otherwise
     */
    void showPivotCoordCross(bool show);

    /**
     * @brief toggles the highlightning of this BodySegment
     */
    void toggleHighlight();
protected:

    /**
     * @brief creates the Channel s of this BodySegment
     */
    void createChannels();

    /**
     * @brief creates the data structures necessary to highlight a BodySegment
     */
    void createHighlightStructures();
    virtual void traverse(osg::NodeVisitor &nv);
    BodySegmentSignalHandler *m_channelHandler;
    Body *m_body; //!< The Body this BodySegment belongs to
    bool m_showPivotCoordCross;
    bool m_pivotCoordCrossCreated;
    virtual ~BodySegment();
    SegmentShape *m_segmentShape;
    bool m_restMatrixDirty;
    bool m_isHiglighted;
    bool m_shouldHighlight;
    bool m_highlghtedStructuresCreated;
    osg::ref_ptr<osg::Geode> m_geode; /**< the geometry node that holds the ShapeDrawable */
    osg::ref_ptr<osg::MatrixTransform> m_transformNode;
    osg::ref_ptr<osg::Switch> m_switchPivotCross; /**< toogles the coordinate cross */
    osg::ref_ptr<osg::Switch> m_switchHighlight; /**< toogles the highlighting */
    osg::ref_ptr<osgFX::Outline> m_highlightOutline; /**< Outline effect for highlighting this BodySegment */
    Channel *m_color; //!< segement´s color Channel
    Channel *m_channelTx; //!< x translation (input and output)
    Channel *m_channelTy; //!< y translation (input and output)
    Channel *m_channelTz; //!< z translation (input and output)
    Channel *m_channelRx; //!< x rotation (input and output)
    Channel *m_channelRy; //!< y rotation (input and output)
    Channel *m_channelRz; //!< z rotation (input and output)
};

#endif // BODYSEGMENT_H
