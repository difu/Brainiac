#ifndef SPHERE_H
#define SPHERE_H
#include "core/agent/body/segment.h"

class Sphere : public Segment
{
public:
    Sphere(quint32 id, Body *body, QString name, QVector3D *restRot, QVector3D *restTrans, qreal radius, Segment *parent=0) ;
    virtual ~Sphere();

    qreal getRestRadius() ;

protected:
    qreal m_restRadius;
    Channel *m_radius;
    virtual void createSegmentChannels();
    virtual void reset();
};

#endif // SPHERE_H
