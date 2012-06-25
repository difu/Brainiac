#include "body.h"
#include "core/agent/agent.h"
#include "core/agent/body/skeletonnode.h"
#include "core/agent/body/skeletonnodesphere.h"
#include "core/agent/body/skeletonnodebox.h"
#include "core/agent/body/skeletongeometrynode.h"
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
    m_rootSkeletonNode=new SkeletonNode(SkeletonNode::NOPRIMITIVE,0,QString("root"),this);
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
        copyBody(body);
    } else {
    }

    //m_rootSegment->addNode(new SkeletonNodeSphere(1));
}

void Body::copyBody(Body *body)
{
    foreach(QGLSceneNode* n, m_rootSkeletonNode->allChildren()) {
        n->deleteLater();
    }
    foreach(QGLSceneNode *n, body->getRootSkeletonNode()->children()) {
        SkeletonNode *skelNode=dynamic_cast<SkeletonNode *> (n);
        if(skelNode) {
            copySkeletonNode(skelNode);
        }
        //qDebug() << "Copied all" << m_rootSegment->allChildren().count();
        //qDumpScene(m_rootSegment);
    }
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
        newNode->setParent(getSkeletonNodeById(((SkeletonNode*)n->parent())->getId()));
        newNode->setRestTranslation(n->getRestTranslation());
        newNode->setRotation(n->getRotation());
        newNode->setTranslation(n->getTranslation());
        newNode->setColor(n->getInitColor());
        newNode->setColorInherited(n->getColorInherited());
        newNode->setRotTransOrder(n->getRotationTranslationOrder());
        addSkeletonNode(newNode,n->getParentId());
        //qDebug() << "ADDED SN" << n->getId() << n->getRestTranslation();
        foreach(QGLSceneNode *sn, n->children()) {
            SkeletonNode *childNode=dynamic_cast<SkeletonNode *> (sn);
            if(childNode)
                copySkeletonNode(childNode);
        }
    }
}

void Body::addSkeletonNode(SkeletonNode *node, quint32 parentId)
{
    if(parentId==0) {
        m_rootSkeletonNode->addNode(node);
        return;
    }
    foreach(QGLSceneNode* n,m_rootSkeletonNode->allChildren()) {
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

QList<SkeletonNode *> Body::getAllSkeletonNodes()
{
    QList<SkeletonNode *> skelNodes;
    foreach(QGLSceneNode *n, m_rootSkeletonNode->allChildren()) {
        SkeletonNode *sn=dynamic_cast<SkeletonNode *>(n);
        if(sn)
            skelNodes.append(sn);
    }
    return skelNodes;
}

SkeletonNode* Body::getSkeletonNodeById(quint32 id)
{
    foreach(QGLSceneNode* n,m_rootSkeletonNode->allChildren()) {
        SkeletonNode *skelNode=dynamic_cast<SkeletonNode *> (n);
        if(skelNode) {
            if(skelNode->getId()==id)
                return skelNode;
        }
    }
    return 0;

}

void Body::renderSilhouettes(bool render)
{
    foreach(QGLSceneNode *n,m_rootSkeletonNode->allChildren()) {
        SkeletonGeometryNode *geoNode=dynamic_cast<SkeletonGeometryNode *>(n);
        if(geoNode) {
            geoNode->showSilhouette(render);
        }
    }
}

void Body::updatePosition() {
    if(m_agent)
        m_rootSkeletonNode->setPosition(*m_agent->getPosition());

}

Body::~Body()
{
    foreach(QGLSceneNode* n, m_rootSkeletonNode->allChildren()) {
        n->deleteLater();
    }
}
