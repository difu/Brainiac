#ifndef SPHERE_H
#define SPHERE_H
#include "core/agent/body/segment.h"

class Sphere : public Segment
{
public:
    Sphere(quint32 id, Body *body, QString name, QVector3D *restRot, QVector3D *restTrans, Segment *parent=0) ;
};

#endif // SPHERE_H
