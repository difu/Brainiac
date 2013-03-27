// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2013  Dirk Fuchs dirkfux@googlemail.com

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

#include "bodysegment.h"
#include "core/agent/body/bodysegmentsignalhandler.h"
#include "core/agent/body/segmentshape.h"
#include "core/agent/channel.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include <osgUtil/UpdateVisitor>
#include <QObject>

BodySegment::BodySegment(Body *body, SegmentShape *segmentShape):MatrixTransform(),m_body(body),m_segmentShape(segmentShape)
{
    m_channelHandler=new BodySegmentSignalHandler(this);
    m_geode=new osg::Geode;
    m_transformNode=new osg::MatrixTransform();
    m_transformNode->setName("Transform");
    addChild(m_transformNode);
    m_transformNode->setMatrix(m_segmentShape->getTransform()->getMatrix());
    m_transformNode->addChild(m_geode);
    m_geode->addDrawable(m_segmentShape->getShapeDrawable().get());
    m_restMatrixDirty=true;
    setName(m_segmentShape->getName().toStdString());
    createChannels();
    computeRestMatrix();
}

void BodySegment::computeMatrix() {
    m_transformNode->setMatrix(m_segmentShape->getTransform()->getMatrix());
}

void BodySegment::computeRestMatrix() {
    if(m_restMatrixDirty) {
        osg::Matrix m;
        foreach(BrainiacGlobals::RotTrans rotTrans,m_segmentShape->getRotationTranslationOrder()) {
            switch(rotTrans) {
            case BrainiacGlobals::RX:
                //m.rotate()
                m*=osg::Matrix::rotate(BrainiacGlobals::grad2rad(m_segmentShape->getRestRotation().x()+m_channelRx->getValue()),osg::Vec3d(1.0f,0.0f,0.0f));
                break;
            case BrainiacGlobals::RY:
                m*=osg::Matrix::rotate(BrainiacGlobals::grad2rad(m_segmentShape->getRestRotation().y()+m_channelRy->getValue()),osg::Vec3d(0.0f,1.0f,0.0f));
                break;
            case BrainiacGlobals::RZ:
                m*=osg::Matrix::rotate(BrainiacGlobals::grad2rad(m_segmentShape->getRestRotation().z()+m_channelRz->getValue()),osg::Vec3d(0.0f,0.0f,1.0f));
                break;
            case BrainiacGlobals::TX:
                m*=osg::Matrix::translate(m_segmentShape->getRestTranslation().x()+m_channelTx->getValue(),0.0f,0.0f);
                break;
            case BrainiacGlobals::TY:
                m*=osg::Matrix::translate(0.0f,m_segmentShape->getRestTranslation().y()+m_channelTy->getValue(),0.0f);
                break;
            case BrainiacGlobals::TZ:
                m*=osg::Matrix::translate(0.0f,0.0f,m_segmentShape->getRestTranslation().z()+m_channelTz->getValue());
                break;
            }
        }
        this->setMatrix(m);
        //this->setMatrix(osg::Matrix::translate(25.0f, 0.0f, 0.0f ));
        m_restMatrixDirty=false;
    }
}

void BodySegment::createChannels()
{
    QString segName(getName().c_str());
    //qDebug() << __PRETTY_FUNCTION__ << "creating channels for Segment" << segName;
    m_channelRx=new Channel();
    QObject::connect(m_channelRx,SIGNAL(valueChanged(qreal)),m_channelHandler,SLOT(restMatrixChanged()),Qt::DirectConnection);
    m_body->getAgent()->addOutputChannel(m_channelRx,segName % ":rx");

    m_channelRy=new Channel();
    QObject::connect(m_channelRy,SIGNAL(valueChanged(qreal)),m_channelHandler,SLOT(restMatrixChanged()),Qt::DirectConnection);
    m_body->getAgent()->addOutputChannel(m_channelRy,segName % ":ry");

    m_channelRz=new Channel();
    QObject::connect(m_channelRz,SIGNAL(valueChanged(qreal)),m_channelHandler,SLOT(restMatrixChanged()),Qt::DirectConnection);
    m_body->getAgent()->addOutputChannel(m_channelRz,segName % ":rz");

    m_channelTx=new Channel();
    QObject::connect(m_channelTx,SIGNAL(valueChanged(qreal)),m_channelHandler,SLOT(restMatrixChanged()),Qt::DirectConnection);
    m_body->getAgent()->addOutputChannel(m_channelTx,segName % ":tx");

    m_channelTy=new Channel();
    QObject::connect(m_channelTy,SIGNAL(valueChanged(qreal)),m_channelHandler,SLOT(restMatrixChanged()),Qt::DirectConnection);
    m_body->getAgent()->addOutputChannel(m_channelTy,segName % ":ty");

    m_channelTz=new Channel();
    QObject::connect(m_channelTz,SIGNAL(valueChanged(qreal)),m_channelHandler,SLOT(restMatrixChanged()),Qt::DirectConnection);
    m_body->getAgent()->addOutputChannel(m_channelTz,segName % ":tz");

    m_color=new Channel();
    m_color->init(0); //!< @todo color connect
    m_body->getAgent()->addOutputChannel(m_color,segName % ":color");

}

void BodySegment::traverse(osg::NodeVisitor &nv)
{
    //osgUtil::UpdateVisitor *uv=dynamic_cast<osgUtil::UpdateVisitor *> (&nv);
    //if(uv) {
    //    qDebug() << __PRETTY_FUNCTION__;
        computeRestMatrix();
    //}
    osg::MatrixTransform::traverse(nv);
}

BodySegment::~BodySegment()
{
    m_channelHandler->deleteLater();
}
