#ifndef BODYSEGMENTTRANSFORMATIONVISITOR_H
#define BODYSEGMENTTRANSFORMATIONVISITOR_H

#include <osg/NodeVisitor>
#include <osg/MatrixTransform>
#include <qglobal.h>

/**
 * @brief calculates the transformation matrix from a body segment to it sbody root segment
 *
 * @class BodySegmentTransformationVisitor bodysegmenttransformationvisitor.h "core/agent/body/bodysegmenttransformationvisitor.h"
 */
class BodySegmentTransformationVisitor : public osg::NodeVisitor
{
public:
    BodySegmentTransformationVisitor();
    /**
     * @brief returns the Transformation matrix that transforms the segment node to its body root
     *
     * @fn getMatrix
     * @return osg::Matrix
     */
    osg::Matrix &getMatrix() const;

    virtual void apply( osg::MatrixTransform &node ) {
        m_transformMatrix *= node.getMatrix();
        traverse( node );

    }
    virtual void apply( osg::Billboard &node ){
        //traverse( node );
    }
    virtual void apply( osg::PositionAttitudeTransform &node)
    {
        Q_UNUSED(node);
    }

private:
    osg::Matrix m_transformMatrix; /**< the current transformation matrix */
};

#endif // BODYSEGMENTTRANSFORMATIONVISITOR_H
