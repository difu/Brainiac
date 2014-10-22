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

    /**
     * @brief Computes the rest matrix of this BodySegment
     * It is only calculated if any parameters has changed
     * @sa m_restMatrixDirty
     * @sa setRestMatrixDirty()
     */
    void computeRestMatrix();

    /**
     * @brief Computes the matrix that offsets the geometry of this BodySegment
     * called e.g. from @sa m_signalHandler
     * @sa m_restMatrixDirty
     */
    void computeMatrix();

    /**
     * @brief the Tx Channel
     *
     * @return Channel the Tx channel
     */
    Channel* getChannelTx() { return m_oTx; }

    /**
     * @brief the Ty Channel
     *
     * @return Channel the Ty channel
     */
    Channel* getChannelTy() { return m_oTy; }

    /**
     * @brief the Tz Channel
     *
     * @return Channel the Tz channel
     */
    Channel* getChannelTz() { return m_oTz; }

    /**
     * @brief the Rx Channel
     *
     * @return Channel the Rx channel
     */
    Channel* getChannelRx() { return m_oRx; }

    /**
     * @brief the Ry Channel
     *
     * @return Channel the Ry channel
     */
    Channel* getChannelRy() { return m_oRy; }

    /**
     * @brief the Rz Channel
     *
     * @return Channel the Rz channel
     */
    Channel* getChannelRz() { return m_oRz; }

    /**
     * @brief return the color channel
     *
     * @fn getColorChannel
     * @return Channel the color channel
     */
    Channel* getColorChannel() { return m_oColor; }

    /**
     * @brief returns the unique id of this BodySegment
     * @return quint32 the id
     */
    quint32 getId() const { return m_segmentShape->getId(); }

    /**
     * @brief returns the parent id for this BodySegment
     * @return quint32 the id
     */
    quint32 getParentId() const { return m_segmentShape->getParentId(); }

    /**
     * @brief returns a pointer of the SegmentShape
     * @return SegmentShape* pointer
     */
    SegmentShape *getSegmentShape() const { return m_segmentShape; }

    /**
     * @brief returns a shared pointer of the shape drawable
     * @return osg::ref_ptr<osg::ShapeDrawable>
     */
    osg::ref_ptr<osg::ShapeDrawable> getShapeDrawable() const { return m_shapeDrawable; }
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
     * @brief resets this BodySegment
     *
     * the rest channels are set to their defualt values
     */
    void reset();

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
    BodySegmentSignalHandler *m_signalHandler; /**< receives and emits signals for this BodySegment */
    Body *m_body; //!< The Body this BodySegment belongs to
    bool m_showPivotCoordCross;
    bool m_pivotCoordCrossCreated;
    virtual ~BodySegment();
    SegmentShape *m_segmentShape;
    osg::Matrix m_restMatrix; /**< the rest matrix */
    bool m_restMatrixApplied; /**< if false, the rest matrix will be set */
    bool m_restMatrixDirty; /**< if true, the rest matrix will be newly calculated */
    bool m_isHiglighted;
    bool m_shouldHighlight;
    bool m_highlghtedStructuresCreated;
    osg::ref_ptr<osg::ShapeDrawable> m_shapeDrawable; /**< the node that holds the geometry */
    osg::ref_ptr<osg::Geode> m_geode; /**< the geometry node that holds the ShapeDrawable */
    osg::ref_ptr<osg::MatrixTransform> m_transformNode; /**< transformation node that offsets the Segmet´s geometry */
    osg::ref_ptr<osg::Switch> m_switchPivotCross; /**< toggles the coordinate cross */
    osg::ref_ptr<osg::Switch> m_switchHighlight; /**< toggles the highlighting */
    osg::ref_ptr<osgFX::Outline> m_highlightOutline; /**< Outline effect for highlighting this BodySegment */
    Channel *m_oColor; //!< segement´s color Channel
    Channel *m_oTx; //!< x translation channel (output)
    Channel *m_oTy; //!< y translation channel (output)
    Channel *m_oTz; //!< z translation channel (output)
    Channel *m_oRx; //!< x rotation channel (output)
    Channel *m_oRy; //!< y rotation channel (output)
    Channel *m_oRz; //!< z rotation channel (output)
};

#endif // BODYSEGMENT_H
