#include "body.h"
#include "core/agent/agent.h"
#include "core/agent/body/segment.h"
#include "core/agent/body/sphere.h"
#include "core/agent/channel.h"

#include <QtOpenGL>
#include <QColor>
#include <glu.h>
#include <GLUT/glut.h>

Body::Body(Agent *agent,Body *body)
{
    m_agent=agent;
    // if we have a body, clone it
    if(body) {
        foreach(Segment *seg, body->getSegments()) {
            if(seg->getType()==Segment::SPHERE) {
                Sphere *origSphere=(Sphere*)seg;
                QVector3D *rot=new QVector3D(origSphere->getRestRotation()->x(),origSphere->getRestRotation()->y(),origSphere->getRestRotation()->z());
                QVector3D *trans=new QVector3D(origSphere->getRestTranslation()->x(),origSphere->getRestTranslation()->y(),origSphere->getRestTranslation()->z());
                qreal color=origSphere->getColor()->getValue();
                bool colorInherited=origSphere->isColorInherited();
                Sphere *newSphere=new Sphere(origSphere->getId(),this,origSphere->getName(),rot,trans,origSphere->getRestRadius());
                newSphere->setParentId(origSphere->getParentId());
                newSphere->getColor()->init(color);
                newSphere->setColorInherited(colorInherited);
                this->addSegment(newSphere);
            } else {
                qDebug() <<  __PRETTY_FUNCTION__ << "missing segment type" << seg->getType();
            }
        }
    }
}

void Body::addSegment(Segment *segment)
{
    m_segments.append(segment);
}

Agent* Body::getAgent()
{
    return m_agent;
}

Segment * Body::getSegment(quint32 id)
{
    foreach(Segment *seg,m_segments) {
        if(seg->getId()==id)
            return seg;
    }
    return 0;
}

quint32 Body::getSegmentId(Segment *seg)
{
    foreach(Segment *segment,m_segments) {
        if(segment==seg)
            return seg->getId();
    }
    return 0;
}

QList<Segment *> Body::getSegments()
{
    return m_segments;
}

void Body::renderGL()
{
    foreach(Segment *seg, this->getSegments()) {
        if(seg->isRootSegment()) {
            glPushMatrix();
//            glRotated(m_agent->getRotation()->x(),1,0,0);
//            glRotated(m_agent->getRotation()->y(),0,1,0);
//            glRotated(m_agent->getRotation()->z(),0,0,1);
            glTranslated(m_agent->getPosition()->x(),m_agent->getPosition()->y(),m_agent->getPosition()->z());
            glRotated(m_agent->getRotation()->x(),1,0,0);
            glRotated(m_agent->getRotation()->y(),0,1,0);
            glRotated(m_agent->getRotation()->z(),0,0,1);
            seg->renderGL();
            glPopMatrix();
        }
    }
}


