#include "skeletonnode.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include "core/agent/channel.h"
#include "core/agent/body/skeletongeometrynode.h"

#include "qglbuilder.h"
#include "qglcube.h"
#include "qglsphere.h"
#include "qglcylinder.h"
#include <qgraphicstranslation3d.h>
#include <qgraphicsrotation3d.h>
#include <qgraphicsscale3d.h>
#include "qglpainter.h"
#include <QColor>
#include "qgeometrydata.h">

SkeletonNode::SkeletonNode(SegmentType type, quint32 id, const QString &name, Body *body, QObject *parent) : QGLSceneNode(parent), m_type(type), m_body(body)
{
    setObjectName(name);
    m_id=id;
    bool hasGeometry=true;
    QGLBuilder builder;
    QGLSceneNode *node;
    SkeletonGeometryNode *nodeExtractedGeometry=new SkeletonGeometryNode();
    switch(type) {
    case BOX:
        builder << QGL::Faceted;
        builder << QGLCube(1.0f);
        node=builder.finalizedSceneNode();
        nodeExtractedGeometry->setGeometry(node->children().at(0)->geometry());
        nodeExtractedGeometry->setCount(node->children().at(0)->count());
        nodeExtractedGeometry->setObjectName("Geometry Node BOX");
        break;
    case SPHERE:
        builder << QGL::Faceted;
        builder << QGLSphere(1.0f);

        node=builder.finalizedSceneNode();
        nodeExtractedGeometry->setGeometry(node->children().at(0)->geometry());
        nodeExtractedGeometry->setCount(node->children().at(0)->count());
        nodeExtractedGeometry->setObjectName("Geometry Node SPHERE");
        break;
    default:
        node=builder.finalizedSceneNode();
        hasGeometry=false;
        break;
    }
    node->deleteLater();

    if(type!=SkeletonNode::NOPRIMITIVE) {
        QGLBuilder b2;
        b2 << QGL::Faceted;
        b2 << QGLCube(0.5f);
        QGLSceneNode *node=b2.finalizedSceneNode()->children().at(0);
        setGeometry(node->geometry());
        setCount(node->count());
        //this->addNode(b2.finalizedSceneNode());
        delete node;
    }

    m_geometryNode = nodeExtractedGeometry;
    this->addNode(nodeExtractedGeometry);

    m_segRestTransX=new QGraphicsTranslation3D();
    m_segRestTransY=new QGraphicsTranslation3D();
    m_segRestTransZ=new QGraphicsTranslation3D();

    m_segRestRotX=new QGraphicsRotation3D();
    m_segRestRotY=new QGraphicsRotation3D();
    m_segRestRotZ=new QGraphicsRotation3D();
    m_segRestRotX->setAxis(QVector3D(1.0f,0.0f,0.0f));
    m_segRestRotY->setAxis(QVector3D(0.0f,1.0f,0.0f));
    m_segRestRotZ->setAxis(QVector3D(0.0f,0.0f,1.0f));

    addTransform(m_segRestRotZ);
    addTransform(m_segRestRotY);
    addTransform(m_segRestRotX);

    addTransform(m_segRestTransX);
    addTransform(m_segRestTransY);
    addTransform(m_segRestTransZ);

    m_segScale=new QGraphicsScale3D();
    m_scale=QVector3D(m_segScale->scale());
    m_geometryNode->addTransform(m_segScale);

    m_segRotX=new QGraphicsRotation3D();
    m_segRotY=new QGraphicsRotation3D();
    m_segRotZ=new QGraphicsRotation3D();
    m_segRotX->setAxis(QVector3D(1.0f,0.0f,0.0f));
    m_segRotY->setAxis(QVector3D(0.0f,1.0f,0.0f));
    m_segRotZ->setAxis(QVector3D(0.0f,0.0f,1.0f));
    m_geometryNode->addTransform(m_segRotX);
    m_geometryNode->addTransform(m_segRotY);
    m_geometryNode->addTransform(m_segRotZ);

    m_segTrans=new QGraphicsTranslation3D();
    m_segTrans->setTranslate(m_translation);
    m_geometryNode->addTransform(m_segTrans);

    createChannels();
}

void SkeletonNode::channelRxChanged(qreal value){
    m_segRestRotX->setAngle(m_restRotation.x()+value);
}

void SkeletonNode::channelRyChanged(qreal value){
    m_segRestRotY->setAngle(m_restRotation.y()+value);
}

void SkeletonNode::channelRzChanged(qreal value){
    m_segRestRotZ->setAngle(m_restRotation.z()+value);
}

void SkeletonNode::createChannels()
{
    m_channelRx=new Channel();
    connect(m_channelRx,SIGNAL(valueChanged(qreal)),this,SLOT(channelRxChanged(qreal)),Qt::DirectConnection);
    m_body->getAgent()->addOutputChannel(m_channelRx,this->objectName() % ":rx");

    m_channelRy=new Channel();
    connect(m_channelRy,SIGNAL(valueChanged(qreal)),this,SLOT(channelRyChanged(qreal)),Qt::DirectConnection);
    m_body->getAgent()->addOutputChannel(m_channelRy,this->objectName() % ":ry");

    m_channelRz=new Channel();
    connect(m_channelRz,SIGNAL(valueChanged(qreal)),this,SLOT(channelRzChanged(qreal)),Qt::DirectConnection);
    m_body->getAgent()->addOutputChannel(m_channelRz,this->objectName() % ":rz");

    m_color=new Channel();
    m_color->init(0);
    m_body->getAgent()->addOutputChannel(m_color,this->objectName() % ":color");

}

void SkeletonNode::drawGeometry(QGLPainter *painter)
{
    QGLSceneNode::drawGeometry(painter);
}

bool SkeletonNode::getColorInherited() const
{
    return m_color->isInherited();
}

QVector3D SkeletonNode::getScale() const {
    return m_scale;
}

quint32 SkeletonNode::getParentId() const {
    if(this->parent()) {
        SkeletonNode *n=dynamic_cast<SkeletonNode *> (this->parent());
        if(n) {
            return(n->getId());
        }
    }
    return 0;
}

QVector3D SkeletonNode::getRestRotation() const
{
    return m_restRotation;
}

QVector3D SkeletonNode::getRestTranslation() const
{
    return m_restTranslation;
}

QMatrix4x4 SkeletonNode::transform() const
{
    QMatrix4x4 m;
    if (!this->localTransform().isIdentity())
        m *= this->localTransform();
    for (int index = this->transforms().size() - 1; index >= 0; --index)
        this->transforms().at(index)->applyTo(&m);
    return m;
}

QList<BrainiacGlobals::RotTrans> SkeletonNode::getRotationTranslationOrder() const
{
    QList<BrainiacGlobals::RotTrans> l;
    foreach(QGraphicsTransform3D *rt,this->transforms()) {
        if(rt==m_segRestRotX) {
            l.append(BrainiacGlobals::RX);
        } else if(rt==m_segRestRotY) {
            l.append(BrainiacGlobals::RY);
        } else if(rt==m_segRestRotZ) {
            l.append(BrainiacGlobals::RZ);
        } else if(rt==m_segRestTransX) {
            l.append(BrainiacGlobals::TX);
        } else if(rt==m_segRestTransY) {
            l.append(BrainiacGlobals::TY);
        } else if(rt==m_segRestTransZ) {
            l.append(BrainiacGlobals::TZ);
        }
    }
    if(l.count()!=6)
        qWarning()<< __PRETTY_FUNCTION__ << "There are less or more than 6 rots/trans! Agent ID" << m_body->getAgent()->getId();
    return l;
}

void SkeletonNode::setColor(qreal color)
{
    m_initColor=color;
    m_color->setValue(qBound((qreal)0.0,color,(qreal)1.0));
    SkeletonGeometryNode *n=dynamic_cast<SkeletonGeometryNode *>(m_geometryNode);
    if(n) {
        n->setColor(BrainiacGlobals::getColorFromBrainiacColorValue(m_color->getValue()));
    }
}

void SkeletonNode::setColorInherited(bool inherited)
{
    if(this->parent()) {
        if(inherited) {
            m_color->changeValue(((SkeletonNode *)this->parent())->getColor()->getValue());
        }
        m_color->setInherited(((SkeletonNode *)this->parent())->getColor(),inherited);
    } else {
        if(inherited) {
            m_color->changeValue(m_body->getAgent()->getColor()->getValue());
        }
        m_color->setInherited(m_body->getAgent()->getColor(),inherited);
    }
}

void SkeletonNode::setRestRotation(const QVector3D &rotation)
{
    if(m_restRotation!=rotation) {
        m_restRotation=rotation;
        m_segRestRotX->setAngle(m_restRotation.x());
        m_segRestRotY->setAngle(m_restRotation.y());
        m_segRestRotZ->setAngle(m_restRotation.z());
    }
}

void SkeletonNode::setRestTranslation(const QVector3D &translation)
{
    if(translation!=m_restTranslation) {
        m_restTranslation=translation;
        m_segRestTransX->setTranslate(QVector3D(translation.x(),0.0f,0.0f));
        m_segRestTransY->setTranslate(QVector3D(0.0f,translation.y(),0.0f));
        m_segRestTransZ->setTranslate(QVector3D(0.0f,0.0f,translation.z()));
    }
}

void SkeletonNode::setRotation(const QVector3D &rotation)
{
    if(m_rotation!=rotation) {
        m_rotation=rotation;
        m_segRotX->setAngle(m_rotation.x());
        m_segRotY->setAngle(m_rotation.y());
        m_segRotZ->setAngle(m_rotation.z());
    }
}

void SkeletonNode::setTranslation(const QVector3D &translation)
{
    if(translation!=m_translation) {
        m_translation=translation;
        m_segTrans->setTranslate(m_translation);
    }
}

void SkeletonNode::setScale(const QVector3D &scale)
{
    if(scale!=m_scale) {
        m_scale=scale;
        m_segScale->setScale(scale);
    }
}

void SkeletonNode::setRotTransOrder(const QList<BrainiacGlobals::RotTrans> &l)
{
    if(l.count()!=6) {
        qWarning()<< __PRETTY_FUNCTION__ << "there are less or more than SIX rot trans items! nothing is changed! Agent Id "<< m_body->getAgent()->getId();
        return;
    }
    QList<QGraphicsTransform3D *> newOrderList;
    foreach(BrainiacGlobals::RotTrans rt, l) {
        switch (rt) {
        case BrainiacGlobals::RX:
            newOrderList.append(m_segRestRotX);
            break;
        case BrainiacGlobals::RY:
            newOrderList.append(m_segRestRotY);
            break;
        case BrainiacGlobals::RZ:
            newOrderList.append(m_segRestRotZ);
            break;
        case BrainiacGlobals::TX:
            newOrderList.append(m_segRestTransX);
            break;
        case BrainiacGlobals::TY:
            newOrderList.append(m_segRestTransY);
            break;
        case BrainiacGlobals::TZ:
            newOrderList.append(m_segRestTransZ);
            break;
        }
    }
    setTransforms(newOrderList);
}

SkeletonNode::~SkeletonNode()
{
    delete m_segRestRotX;
    delete m_segRestRotY;
    delete m_segRestRotZ;

    delete m_segRotX;
    delete m_segRotY;
    delete m_segRotZ;

    delete m_segRestTransX;
    delete m_segRestTransY;
    delete m_segRestTransZ;

    delete m_segTrans;
    delete m_segScale;

    m_body->getAgent()->deleteChannel(m_color);

//    m_body->getAgent()->deleteChannel(m_channelTx);
//    m_body->getAgent()->deleteChannel(m_channelTy);
//    m_body->getAgent()->deleteChannel(m_channelTz);
    m_body->getAgent()->deleteChannel(m_channelRx);
    m_body->getAgent()->deleteChannel(m_channelRy);
    m_body->getAgent()->deleteChannel(m_channelRz);
}
