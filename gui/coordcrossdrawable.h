#ifndef COORDCROSSDRAWABLE_H
#define COORDCROSSDRAWABLE_H

#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>
#ifdef BRAINIAC_SUPPRESS_THIRD_PARTY_WARNINGS
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
#include <osgViewer/Viewer>
#ifdef BRAINIAC_SUPPRESS_THIRD_PARTY_WARNINGS
    #pragma clang diagnostic pop
#endif


/**
 * @brief A simple coordinate cross
 *
 * represents a simple coordinate cross
 */
class CoordCrossDrawable  : public osg::Drawable
{
public:
    CoordCrossDrawable();
    CoordCrossDrawable( const CoordCrossDrawable& copy,
                        const osg::CopyOp&
                        copyop=osg::CopyOp::SHALLOW_COPY ) : osg::Drawable(copy, copyop) {}
    META_Object( osg, CoordCrossDrawable );
    virtual osg::BoundingBox computeBound() const;
    virtual void drawImplementation(osg::RenderInfo& renderInfo) const;
};

#endif // COORDCROSSDRAWABLE_H
