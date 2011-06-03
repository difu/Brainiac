#include "segment.h"
#include "core/brainiaccolor.h"
#include "core/agent/body/body.h"
#include "core/agent/agent.h"
#include "core/agent/channel.h"
#include <QVector3D>
#include <QtOpenGL>


Segment::Segment(SegmentType type, quint32 id, Body *body, QString name, QVector3D *restRot, QVector3D *restTrans, Segment *parent)
{
    m_type=type;
    m_id=id;
    m_parent=parent;
    //m_scale=1.0;
    m_body=body;
    m_name=name;
    m_restRotation=restRot;
    m_restTranslation=restTrans;
    //m_color=new BrainiacColor();
    //m_color=0.5; //!< \todo color management
    createSegmentChannels();
    if(parent) {
        parent->addChild(this);
    }
}
Segment::~Segment()
{
    delete m_restRotation;
    delete m_restTranslation;
    m_body->getAgent()->deleteChannel(m_tx);
    m_body->getAgent()->deleteChannel(m_ty);
    m_body->getAgent()->deleteChannel(m_tz);
    m_body->getAgent()->deleteChannel(m_rx);
    m_body->getAgent()->deleteChannel(m_ry);
    m_body->getAgent()->deleteChannel(m_rz);
}

/** \brief add a segment to this sigment´s children

**/
void Segment::addChild(Segment *segment)
{
    m_children.append(segment);
}

/** \brief add a segment to this sigment´s children

**/
void Segment::addChildId(quint32 id)
{
    Segment *seg=m_body->getSegment(id);
    if(seg)
        addChild(seg);
    else
        qCritical() << __PRETTY_FUNCTION__ << "Error adding child with id " << id << "! No such id in list!";
}



void Segment::createSegmentChannels()
{
    m_tx=new Channel();
    m_tx->init(m_restTranslation->x());
    QString name=m_name % ":tx";
    m_body->getAgent()->addInputChannel(m_tx,name);

    m_ty=new Channel();
    m_ty->init(m_restTranslation->y());
    name=m_name % ":ty";
    m_body->getAgent()->addInputChannel(m_ty,name);

    m_tz=new Channel();
    m_tz->init(m_restTranslation->z());
    name=m_name % ":tz";
    m_body->getAgent()->addInputChannel(m_tz,name);

    m_rx=new Channel();
    m_rx->init(m_restRotation->x());
    name=m_name % ":rx";
    m_body->getAgent()->addInputChannel(m_rx,name);

    m_ry=new Channel();
    m_ry->init(m_restRotation->y());
    name=m_name % ":ry";
    m_body->getAgent()->addInputChannel(m_ry,name);

    m_rz=new Channel();
    m_rz->init(m_restRotation->z());
    name=m_name % ":rz";
    m_body->getAgent()->addInputChannel(m_rz,name);

    m_color=new Channel();
    m_color->init(0);
    name=m_name % ":color";
    m_body->getAgent()->addOutputChannel(m_color,name);
}

Segment* Segment::getParent() {
    return m_parent;
}

quint32 Segment::getParentId()
{
    if(m_parent)
        return m_body->getSegmentId(m_parent);
    else
        return 0;
}

bool Segment::isRootSegment()
{
    if(m_parent==0) {
        return true;
    } else {
        return false;
    }
}

void Segment::renderGL()
{
    glTranslated(getTransX()->getValue(),getTransY()->getValue(),getTransZ()->getValue());
    glLineWidth( 3.0 );
    QColor col=BrainiacGlobals::getColorFromBrainiacColorValue(getColor()->getValue());
    glColor3f(col.redF(),col.greenF(),col.blueF());
    this->renderGLSegment();
    foreach(Segment *segChild,getChildren()) {
        segChild->renderGL();
    }
}

/** \brief reset this segment

                resets this segement´s rotation and translation etc. to its rest pose

**/
void Segment::reset()
{
    m_tx->init(m_restTranslation->x());
    m_ty->init(m_restTranslation->y());
    m_tz->init(m_restTranslation->z());

    m_rx->init(m_restRotation->x());
    m_ry->init(m_restRotation->y());
    m_rz->init(m_restRotation->z());
}

void Segment::setColorInherited(bool inherited)
{
    m_inheritColor=inherited;
    if(m_parent) {
        if(inherited)
            m_color->changeValue(m_parent->getColor()->getValue());
        m_color->setInherited(m_parent->getColor(),m_inheritColor);
    } else {
        if(inherited)
            m_color->changeValue(m_body->getAgent()->getColor()->getValue());
        m_color->setInherited(m_body->getAgent()->getColor(),m_inheritColor);
    }
}

/** \brief set this segment´s name

**/
void Segment::setName(const QString &name) {
    m_name=name;
}

/** \brief set this segment´s parent

**/
void Segment::setParentId(quint32 id)
{
    if(m_body->getSegment(id)) {
        m_body->getSegment(id)->addChild(this);
        m_parent=m_body->getSegment(id);
    }
}

/** \brief set this segment´s parent

**/
void Segment::setParent(Segment *segment)
{
    if(segment) {
        segment->addChild(this);
        m_parent=segment;
    }

}
