#include "segment.h"
#include "core/brainiaccolor.h"
#include "core/agent/body/body.h"
#include "core/agent/agent.h"
#include "core/agent/channel.h"
#include <QVector3D>
#include <QtOpenGL>


Segment::Segment(SegmentType type, quint32 id, Body *body, QString name, QVector3D *rot, QVector3D *trans, Segment *parent)
{
    init(type,id,body,name,rot,trans,parent);
}
Segment::~Segment()
{
    delete m_rotation;
    delete m_translation;
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
    m_tx->init(m_translation->x());
    QString name=m_name % ":tx";
    m_body->getAgent()->addInputChannel(m_tx,name);

    m_ty=new Channel();
    m_ty->init(m_translation->y());
    name=m_name % ":ty";
    m_body->getAgent()->addInputChannel(m_ty,name);

    m_tz=new Channel();
    m_tz->init(m_translation->z());
    name=m_name % ":tz";
    m_body->getAgent()->addInputChannel(m_tz,name);

    m_rx=new Channel();
    m_rx->init(m_rotation->x());
    name=m_name % ":rx";
    m_body->getAgent()->addInputChannel(m_rx,name);

    m_ry=new Channel();
    m_ry->init(m_rotation->y());
    name=m_name % ":ry";
    m_body->getAgent()->addInputChannel(m_ry,name);

    m_rz=new Channel();
    m_rz->init(m_rotation->z());
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

void Segment::init(SegmentType type, quint32 id, Body *body, QString name, QVector3D *restRot, QVector3D *restTrans, Segment *parent)
{
    m_type=type;
    m_id=id;
    m_parent=parent;
    //m_scale=1.0;
    m_body=body;
    m_name=name;
    m_rotation=restRot;
    m_translation=restTrans;
    //m_color=new BrainiacColor();
    //m_color=0.5; //!< \todo color management
    createSegmentChannels();
    if(parent) {
        parent->addChild(this);
    }
}

bool Segment::isColorInherited()
{
    return m_color->isInherited();
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
//    static GLint vp[4];
//    glGetIntegerv(GL_VIEWPORT,vp);
//    glTranslated(getTransX()->getValue(),getTransY()->getValue(),getTransZ()->getValue());
//    glLineWidth( 3.0 );
//    QColor col=BrainiacGlobals::getColorFromBrainiacColorValue(getColor()->getValue());
//    glColor3f(col.redF(),col.greenF(),col.blueF());
//    //this->renderGLSegment();
//    //
//    glClear(GL_STENCIL_BUFFER_BIT);
//    int i=0;
//    int winWidth = vp[2];
//    int winHeight = vp[3];
//    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//    glEnable(GL_STENCIL_TEST);
//    glStencilFunc(GL_ALWAYS, 1, 1);
//    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//    glDisable(GL_DEPTH_TEST);  /* so the depth buffer doesn't change */
//    for (i = -1; i < 2; i += 2) {  /* set stencil around object */
//      glViewport(i, 0, winWidth + i, winHeight);
//      this->renderGLSegment();
//    }
//    for (i = -1; i < 2; i += 2) {
//      glViewport(0, i, winWidth, winHeight + i);
//      this->renderGLSegment();
//    }

//    /* cut out stencil where object is */
//    glViewport(0, 0, winWidth, winHeight);
//    glStencilFunc(GL_ALWAYS, 0, 0);
//    this->renderGLSegment();

//    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

//    glStencilFunc(GL_EQUAL, 1, 1);

//    glDisable(GL_LIGHTING);
//glColor3f(1.f, 1.f, 1.f);
//glRecti(-15000, -15000, 15000, 15000);
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHTING);
//    glDisable(GL_STENCIL_TEST);
//    //

    // No Silhouette
    glTranslated(getTransX()->getValue(),getTransY()->getValue(),getTransZ()->getValue());
    glLineWidth( 3.0 );
    QColor col=BrainiacGlobals::getColorFromBrainiacColorValue(getColor()->getValue());
    glColor3f(col.redF(),col.greenF(),col.blueF());
    this->renderGLSegment();


    foreach(Segment *segChild,getChildren()) {
        segChild->renderGL();
    }
}

void Segment::reset()
{
    m_tx->init(m_translation->x());
    m_ty->init(m_translation->y());
    m_tz->init(m_translation->z());

    m_rx->init(m_rotation->x());
    m_ry->init(m_rotation->y());
    m_rz->init(m_rotation->z());
}

void Segment::setColorInherited(bool inherited)
{
    if(m_parent) {
        if(inherited)
            m_color->changeValue(m_parent->getColor()->getValue());
        m_color->setInherited(m_parent->getColor(),inherited);
    } else {
        if(inherited)
            m_color->changeValue(m_body->getAgent()->getColor()->getValue());
        m_color->setInherited(m_body->getAgent()->getColor(),inherited);
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
    } else if(id<=0){
        m_parent=0;
    } else {
        qDebug() << __PRETTY_FUNCTION__ << "Invalid id";
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

void Segment::setSegmentColor(qreal color)
{
    m_segmentColor=qBound((qreal)0.0,color,(qreal)1.0);
}
