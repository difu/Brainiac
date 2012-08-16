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


#include "skeletonnodesphere.h"
#include "core/agent/channel.h"
#include "core/agent/body/body.h"
#include "core/agent/agent.h"


SkeletonNodeSphere::SkeletonNodeSphere(quint32 id, const QString &name, Body *body):SkeletonNode(SkeletonNode::SPHERE, id, name, body)
{
    createChannels();
    //SkeletonNode::createChannels();
}

void SkeletonNodeSphere::createChannels()
{
    m_radius=new Channel(0,100);
    m_radius->init(m_scale.x());
    QString name=this->objectName() % ":radius";
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
