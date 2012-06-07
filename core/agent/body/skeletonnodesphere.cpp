#include "skeletonnodesphere.h"
#include "core/agent/channel.h"
#include "core/agent/body/body.h"
#include "core/agent/agent.h"


SkeletonNodeSphere::SkeletonNodeSphere(quint32 id, const QString &name, Body *body):SkeletonNode(SkeletonNode::SPHERE, id, name, body)
{
    createChannels();
}

void SkeletonNodeSphere::createChannels()
{
    m_radius=new Channel(0,100);
    m_radius->init(m_scale.x());
    QString name=m_name % ":radius";
    m_body->getAgent()->addInputChannel(m_radius,name);
    m_body->getAgent()->addOutputChannel(m_radius,name);
    connect(m_radius,SIGNAL(valueChanged(qreal)),this,SLOT(channelRadiusChanged(qreal)),Qt::DirectConnection);
}

void SkeletonNodeSphere::setRadius(qreal radius)
{
    radius=qAbs(radius);
    setScale(QVector3D(radius,radius,radius));
}

void SkeletonNodeSphere::channelRadiusChanged(qreal value)
{
    m_segScale->setScale(QVector3D(value, value, value));
}
