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
    m_switchHighlight=new osg::Switch;
    m_transformNode=new osg::MatrixTransform();
    m_transformNode->setName("Transform");
    addChild(m_transformNode);
    m_transformNode->setMatrix(m_segmentShape->getTransform()->getMatrix());
    m_transformNode->addChild(m_switchHighlight);
    m_switchHighlight->addChild(m_geode,true);
    //m_transformNode->addChild(m_geode);
    m_geode->addDrawable(m_segmentShape->getShapeDrawable().get());
    m_restMatrixDirty=true;
    setName(m_segmentShape->getName().toStdString());
    createChannels();
    computeRestMatrix();
    m_showPivotCoordCross=false;
    m_pivotCoordCrossCreated=false;
    m_isHiglighted=false;
    m_highlghtedStructuresCreated=false;
    m_shouldHighlight=false;
    reset();
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
                m*=osg::Matrix::rotate(BrainiacGlobals::grad2rad(m_channelRx->getValue()),osg::Vec3d(1.0,0.0,0.0));
                break;
            case BrainiacGlobals::RY:
                m*=osg::Matrix::rotate(BrainiacGlobals::grad2rad(m_channelRy->getValue()),osg::Vec3d(0.0,1.0,0.0));
                break;
            case BrainiacGlobals::RZ:
                m*=osg::Matrix::rotate(BrainiacGlobals::grad2rad(m_channelRz->getValue()),osg::Vec3d(0.0,0.0,1.0));
                break;
            case BrainiacGlobals::TX:
                m*=osg::Matrix::translate(m_channelTx->getValue(),0.0,0.0);
                break;
            case BrainiacGlobals::TY:
                m*=osg::Matrix::translate(0.0,m_channelTy->getValue(),0.0);
                break;
            case BrainiacGlobals::TZ:
                m*=osg::Matrix::translate(0.0,0.0,m_channelTz->getValue());
                break;
            }
        }
        this->setMatrix(m);
        //this->setMatrix(osg::Matrix::translate(25.0, 0.0, 0.0 ));
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

void BodySegment::createHighlightStructures()
{
    m_highlightOutline=new osgFX::Outline;
    m_highlightOutline->setWidth(2);
    m_highlightOutline->setColor(osg::Vec4(1,1,0,1));
    m_highlightOutline->addChild(m_geode);
    m_switchHighlight->addChild(m_highlightOutline,false);
    m_highlghtedStructuresCreated=true;
}

void BodySegment::showPivotCoordCross(bool show)
{
    if(!m_pivotCoordCrossCreated) {
        m_switchPivotCross=new osg::Switch;
        m_pivotCoordCrossCreated=true;
        osg::Geode *geod=new osg::Geode;
        geod->addDrawable(BrainiacGlobals::CoordCross.get());

        // Switch Lightning off for coord cross
        osg::StateSet *state = geod->getOrCreateStateSet();
        state->setMode( GL_LIGHTING, osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF );

        m_switchPivotCross->addChild(geod,true);
        this->addChild(m_switchPivotCross.get());
        m_pivotCoordCrossCreated=true;
    }
    m_switchPivotCross->setValue(0,show);
}

void BodySegment::reset()
{
    m_channelTx->setValue(m_segmentShape->getRestTranslation().x());
    m_channelTy->setValue(m_segmentShape->getRestTranslation().y());
    m_channelTz->setValue(m_segmentShape->getRestTranslation().z());

    m_channelRx->setValue(m_segmentShape->getRestRotation().x());
    m_channelRy->setValue(m_segmentShape->getRestRotation().y());
    m_channelRz->setValue(m_segmentShape->getRestRotation().z());
    m_restMatrixDirty=true;

}

void BodySegment::toggleHighlight()
{
    m_shouldHighlight=!m_shouldHighlight;
}

void BodySegment::traverse(osg::NodeVisitor &nv)
{
    computeRestMatrix();
    if(m_shouldHighlight&&!m_isHiglighted) {
        if(!m_highlghtedStructuresCreated) {
            createHighlightStructures();
        }
        m_switchHighlight->setValue(0,false);
        m_switchHighlight->setValue(1,true);
        m_isHiglighted=true;
    }
    if(!m_shouldHighlight&&m_isHiglighted) {
        m_switchHighlight->setValue(0,true);
        m_switchHighlight->setValue(1,false);
        m_isHiglighted=false;
    }
    osg::MatrixTransform::traverse(nv);
}

BodySegment::~BodySegment()
{
    m_channelHandler->deleteLater();
    m_body->getAgent()->deleteChannel(m_channelRx);
    m_body->getAgent()->deleteChannel(m_channelRy);
    m_body->getAgent()->deleteChannel(m_channelRz);
    m_body->getAgent()->deleteChannel(m_channelTx);
    m_body->getAgent()->deleteChannel(m_channelTy);
    m_body->getAgent()->deleteChannel(m_channelTz);
    m_body->getAgent()->deleteChannel(m_color);
}
