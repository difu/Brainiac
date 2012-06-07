#include "skeletonnode.h"

#include "qglbuilder.h"
#include "qglcube.h"
#include "qglsphere.h"
#include <qgraphicstranslation3d.h>
#include <qgraphicsrotation3d.h>
#include <qgraphicsscale3d.h>
#include "qglpainter.h"
#include <QColor>

SkeletonNode::SkeletonNode(SegmentType type, quint32 id, const QString &name, Body *body, QObject *parent) : QGLSceneNode(parent), m_type(type), m_id(id), m_name(name), m_body(body)
{
    QGLBuilder builder;
    switch(type) {
    case BOX:
        builder << QGL::Faceted;
        builder << QGLCube(1.0f);
        break;
    case SPHERE:
        builder << QGL::Faceted;
        builder << QGLSphere(1.0f);
        break;
    default:
        m_segmentNode=0;
    }
    m_segmentNode = builder.finalizedSceneNode();
    this->addNode(m_segmentNode);
    m_segRestTrans=new QGraphicsTranslation3D();
    m_segRestRot=new QGraphicsRotation3D();
    m_segScale=new QGraphicsScale3D();
    m_scale=QVector3D(m_segScale->scale());
    m_segmentNode->addTransform(m_segScale);
    addTransform(m_segRestTrans);
    addTransform(m_segRestRot);

}

void SkeletonNode::draw(QGLPainter *painter) {
    painter->setFaceColor(QGL::AllFaces, QColor(255, 00, 0));
    QGLSceneNode::draw(painter);
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

QVector3D SkeletonNode::getRestTranslation() const
{
    return m_restTranslation;
}

void SkeletonNode::setRestTranslation(const QVector3D &translation)
{
    if(translation!=m_translation) {
        m_restTranslation=translation;
        m_segRestTrans->setTranslate(translation);
    }
}

void SkeletonNode::setScale(const QVector3D &scale)
{
    if(scale!=m_scale) {
        m_scale=scale;
        m_segScale->setScale(scale);
    }
}
