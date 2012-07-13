#ifndef SKELETONGEOMETRYNODE_H
#define SKELETONGEOMETRYNODE_H

#include "qglscenenode.h"

/**
 * @brief Represents the drawn geometry of a SkeletonNode (e.g. Sphere)
 * @todo (real) Silhouette drawing
 * @class SkeletonGeometryNode skeletongeometrynode.h "core/agent/body/skeletongeometrynode.h"
 */
class SkeletonGeometryNode : public QGLSceneNode
{
public:
    SkeletonGeometryNode();
    virtual void draw(QGLPainter *painter);
    void showSilhouette(bool show);
    void setColor(QColor col) {m_color=col;showSilhouette(m_showSilhouette);}

protected:
    virtual void drawGeometry(QGLPainter *painter);
    bool m_showSilhouette;
    QColor m_color;
};

#endif // SKELETONGEOMETRYNODE_H
