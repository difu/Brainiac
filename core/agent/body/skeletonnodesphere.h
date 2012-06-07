#ifndef SKELETONNODESPHERE_H
#define SKELETONNODESPHERE_H
#include "skeletonnode.h"
#include "qgraphicsscale3d.h"
class Channel;

class SkeletonNodeSphere : public SkeletonNode
{
    Q_OBJECT
public:
    SkeletonNodeSphere(quint32 id, const QString &name, Body *body);
    /**
     * @brief sets the radius of this SkeletonNodeSphere
     *
     * Sets the initial radius of this sphere. Internally it sets all dimensions of the geometry´s scale to radius
     *
     * @param radius the radius of this sphere
     */
    void setRadius(qreal radius);
    /**
     * @brief returns the radius of this SkeletonNodeSphere
     *
     * Returns the radius. Internally only the x dimension is returned as per definitiony the scale of x=y=z
     *
     * @return qreal the radius
     */
    qreal getRadius() const { return m_segScale->scale().x(); }
protected:
    void createChannels();
    Channel *m_radius;
protected slots:
    void channelRadiusChanged(qreal value);
};

#endif // SKELETONNODESPHERE_H
