#include "body.h"
#include "core/agent/agent.h"
#include "core/agent/body/segment.h"
#include "core/agent/body/sphere.h"
#include "core/agent/body/skeletonnode.h"
#include "core/agent/body/skeletonnodesphere.h"
#include "core/agent/body/skeletonnodebox.h"
#include "core/agent/channel.h"

#include <QtOpenGL>
#include <QColor>
#include <glu.h>
#include <GLUT/glut.h>

#include "qglbuilder.h"
#include "qglcube.h"

Body::Body(Agent *agent,Body *body)
{
    m_agent=agent;
    m_rootSegment=new SkeletonNode(SkeletonNode::NOPRIMITIVE,0,QString("root"),this);
    // if we have a body, clone it
    if(body) {
//        foreach(Segment *seg, body->getSegments()) {
//            if(seg->getType()==Segment::SPHERE) {
//                Sphere *origSphere=(Sphere*)seg;
//                QVector3D *rot=new QVector3D(origSphere->getRestRotation()->x(),origSphere->getRestRotation()->y(),origSphere->getRestRotation()->z());
//                QVector3D *trans=new QVector3D(origSphere->getRestTranslation()->x(),origSphere->getRestTranslation()->y(),origSphere->getRestTranslation()->z());
//                qreal color=origSphere->getColor()->getValue();
//                bool colorInherited=origSphere->isColorInherited();
//                Sphere *newSphere=new Sphere(origSphere->getId(),this,origSphere->getName(),rot,trans,origSphere->getSphereRadius());
//                newSphere->setParentId(origSphere->getParentId());
//                newSphere->getColor()->init(color);
//                newSphere->setColorInherited(colorInherited);
//                this->addSegment(newSphere);
//            } else {
//                qDebug() <<  __PRETTY_FUNCTION__ << "missing segment type" << seg->getType();
//            }
//        }
        foreach(QGLSceneNode *n, body->getRootSkeletonNode()->children()) {
            SkeletonNode *skelNode=dynamic_cast<SkeletonNode *> (n);
            if(skelNode) {
                copySkeletonNode(skelNode);
            }
            //qDebug() << "Copied all" << m_rootSegment->allChildren().count();
            //qDumpScene(m_rootSegment);
        }
    } else {
    }

    //m_rootSegment->addNode(new SkeletonNodeSphere(1));
}

void Body::copySkeletonNode(SkeletonNode *n)
{
    SkeletonNode *newNode=0;
    if(dynamic_cast<SkeletonNodeSphere *> (n)) {
        SkeletonNodeSphere *sphere=(SkeletonNodeSphere *)n;
        SkeletonNodeSphere *newSphere=new SkeletonNodeSphere(sphere->getId(), sphere->getName(), this);
        newSphere->setRadius(sphere->getRadius());
        newNode=newSphere;
    } else if(dynamic_cast<SkeletonNodeBox *> (n)) {
        SkeletonNodeBox *box=(SkeletonNodeBox *)n;
        SkeletonNodeBox *newBox=new SkeletonNodeBox(box->getId(), box->getName(), this);
        newBox->setScale(box->getScale());
        newNode=newBox;
    }
    if(newNode) {
        newNode->setRestTranslation(n->getRestTranslation());
        addSkeletonNode(newNode,n->getParentId());
        //qDebug() << "ADDED SN" << n->getId() << n->getRestTranslation();
        foreach(QGLSceneNode *sn, n->children()) {
            SkeletonNode *childNode=dynamic_cast<SkeletonNode *> (sn);
            if(childNode)
                copySkeletonNode(childNode);
        }
    }
}

void Body::addSegment(Segment *segment)
{
    m_segments.append(segment);
}

void Body::addSkeletonNode(SkeletonNode *node, quint32 parentId)
{
    if(parentId==0) {
        m_rootSegment->addNode(node);
        return;
    }
    foreach(QGLSceneNode* n,m_rootSegment->allChildren()) {
        SkeletonNode *skelNode=dynamic_cast<SkeletonNode *> (n);
        if(skelNode) {
            if(skelNode->getId()==parentId) {
                skelNode->addNode(node);
                //qDebug() << __PRETTY_FUNCTION__ << "Node added";
                return;
            }
        }
    }
    qWarning() << __PRETTY_FUNCTION__ << "No node added!!!";
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

void Body::updatePosition() {
   m_rootSegment->setPosition(*m_agent->getPosition());

}

Body::~Body()
{
    foreach(Segment *seg, this->getSegments()) {
        delete seg;
    }
}
