#ifndef SPHERE_H
#define SPHERE_H
#include "core/agent/body/segment.h"

class Sphere : public Segment
{
public:
    Sphere(quint32 id, Body *body, QString name, QVector3D *rot, QVector3D *trans, qreal radius, Segment *parent=0) ;
    virtual ~Sphere();

    qreal getSphereRadius() ;
    Channel* getRadius();

protected:
    qreal m_sphereRadius;
    Channel *m_radius;
    virtual void createSegmentChannels();
    virtual void renderGLSegment();
    virtual void reset();
};

#endif // SPHERE_H
