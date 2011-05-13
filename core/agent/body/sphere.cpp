#include "sphere.h"

Sphere::Sphere(quint32 id, Body *body, QString name, QVector3D *restRot, QVector3D *restTrans, Segment *parent)
    :Segment(Segment::SPHERE,id,body,name,restRot,restTrans,parent)
{
}
