// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "body.h"
#include "core/scene.h"
#include "core/simulation.h"
#include "core/agent/agent.h"
#include "core/agent/body/animation/animationplayer.h"
#include "core/agent/body/skeletonnode.h"
#include "core/agent/body/skeletonnodesphere.h"
#include "core/agent/body/skeletonnodebox.h"
#include "core/agent/body/skeletongeometrynode.h"
#include "core/agent/channel.h"

#include <QtOpenGL>
#include "qglpainter.h"
#include <QColor>
#include <glu.h>
#include <GLUT/glut.h>

#include "qglbuilder.h"
#include "qglcube.h"

Body::Body(Agent *agent,Body *body)
{
    m_agent=agent;
    m_rootSkeletonNode=new SkeletonNode(SkeletonNode::NOPRIMITIVE,0,QString("root"),this);
    m_animationPlayer=new AnimationPlayer(this);
    // if we have a body, clone it
    if(body) {
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
    m_animationPlayer->setAnimations(body->getAnimations());
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

QHash<quint32,Animation *> * Body::getAnimations()
{
    return m_animationPlayer->getAnimations();
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

SkeletonNode* Body::getRootBone()
{
    SkeletonNode *returnNode=0;
    quint32 count=0;
    foreach(QGLSceneNode *n, m_rootSkeletonNode->children()) {
        SkeletonNode *sn=dynamic_cast<SkeletonNode *> (n);
        if(sn) {
            returnNode=sn;
            count++;
        }
    }
    if(count>1) {
        qWarning() << __PRETTY_FUNCTION__ << "Body has 2 or more root bones!!! returning the last one";
    }
    return returnNode;
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

SkeletonNode* Body::getSkeletonNodeByName(const QString &name)
{
    foreach(QGLSceneNode* n,m_rootSkeletonNode->allChildren()) {
        SkeletonNode *skelNode=dynamic_cast<SkeletonNode *> (n);
        if(skelNode) {
            if(skelNode->objectName()==name)
                return skelNode;
        }
    }
    return 0;
}

void Body::renderSkeleton(QGLPainter *painter)
{
    renderSkeleton(painter,this->getRootSkeletonNode());
}

void Body::renderSkeleton(QGLPainter *painter, SkeletonNode *node) const
{
    //painter->modelViewMatrix().push();
    foreach(QGLSceneNode *n,node->children()) {
        SkeletonNode *skelChildNode=dynamic_cast<SkeletonNode *>(n);
        if(skelChildNode) {
            painter->modelViewMatrix().push();
            QVector3DArray verts;
            painter->modelViewMatrix()*=node->transform();
            if(node->hasPrimitive()) {
                QMatrix4x4 m = skelChildNode->transform();
                QVector3D endVertex=m.map(QVector3D(0,0,0));
                verts.append(QVector3D(0,0,0));
                verts.append(endVertex);
                painter->clearAttributes();
                painter->setStandardEffect(QGL::FlatColor);
                painter->setColor(QColor(170, 202, 0));
                painter->setVertexAttribute(QGL::Position, verts);
                painter->draw(QGL::Lines, 2);
            }
            renderSkeleton(painter,skelChildNode);
            painter->modelViewMatrix().pop();
        }
    }
}

void Body::setAnimations(QHash<quint32, Animation *> *animations)
{
    m_animationPlayer->setAnimations(animations);
}

void Body::showBoneCoordCrosses(bool show)
{
    foreach(QGLSceneNode *n,m_rootSkeletonNode->allChildren()) {
        SkeletonNode *node=dynamic_cast<SkeletonNode *>(n);
        if(node) {
            node->showCoordCross(show);
        }
        m_rootSkeletonNode->showCoordCross(show);
    }
}

void Body::showSilhouettes(bool show)
{
    foreach(QGLSceneNode *n,m_rootSkeletonNode->allChildren()) {
        SkeletonGeometryNode *geoNode=dynamic_cast<SkeletonGeometryNode *>(n);
        if(geoNode) {
            geoNode->showSilhouette(show);
        }
    }
}

void Body::updatePosition() {
    if(m_agent)
        m_rootSkeletonNode->setPosition(*m_agent->getPosition());

}

Body::~Body()
{
//    foreach(QGLSceneNode* n, m_rootSkeletonNode->allChildren()) {
//        delete n;
//    }
    delete m_rootSkeletonNode;
}
