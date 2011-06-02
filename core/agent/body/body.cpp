#include "body.h"
#include "core/agent/agent.h"
#include "core/agent/body/segment.h"
#include "core/agent/body/sphere.h"
#include "core/agent/channel.h"

#include <QtOpenGL>
#include <QColor>
#include <glu.h>
#include <GLUT/glut.h>

Body::Body(Agent *agent)
{
    m_agent=agent;
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
            glTranslated(m_agent->getPosition()->x(),m_agent->getPosition()->y(),m_agent->getPosition()->z());
            seg->renderGL();
            glPopMatrix();
        }
    }
}


void Body::renderSegment(Segment *seg)
{
//    glTranslated(seg->getTransX()->getValue(),seg->getTransY()->getValue(),seg->getTransZ()->getValue());
//    glLineWidth( 3.0 );
//    QColor col=BrainiacGlobals::getColorFromBrainiacColorValue(seg->getColor()->getValue());
//    glColor3f(col.redF(),col.greenF(),col.blueF());
//    if(seg->getType()==Segment::SPHERE) {
//        Sphere *sphere=(Sphere*)seg;
//        glutSolidSphere(sphere->getRadius()->getValue(),20,10);
//    }
//    foreach(Segment *segChild,seg->getChildren()) {
//        renderSegment(segChild);
//    }
//    qDebug() << "Rendered Segment"<<seg->getName();
}
