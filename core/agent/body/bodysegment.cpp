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
    m_signalHandler=new BodySegmentSignalHandler(this);

    osg::TessellationHints *hints=new osg::TessellationHints();
    hints->setDetailRatio(BrainiacGlobals::SegmentTesselationDetailRatio);
    m_shapeDrawable=new osg::ShapeDrawable();
    m_shapeDrawable->setColor( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    m_shapeDrawable->setName("ShapeDrawable");
    m_shapeDrawable->setTessellationHints(hints);

    m_shapeDrawable->setShape(m_segmentShape->getShape().get());
    m_geode=new osg::Geode;
    m_switchHighlight=new osg::Switch;
    m_transformNode=new osg::MatrixTransform();
    m_transformNode->setName("Transform");
    addChild(m_transformNode);
    m_transformNode->setMatrix(m_segmentShape->getTransform()->getMatrix());
    m_transformNode->addChild(m_switchHighlight);
    m_switchHighlight->addChild(m_geode,true);
    //m_transformNode->addChild(m_geode);
    m_geode->addDrawable(m_shapeDrawable);
    m_restMatrixDirty=true;
    m_restMatrixApplied=false;
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
    m_shapeDrawable->dirtyDisplayList();
    m_transformNode->setMatrix(m_segmentShape->getTransform()->getMatrix());
}

void BodySegment::computeRestMatrix() {
    if(m_restMatrixDirty) {
        osg::Matrix m;
        m_restMatrix=m;
        foreach(BrainiacGlobals::RotTrans rotTrans,m_segmentShape->getRotationTranslationOrder()) {
            switch(rotTrans) {
            case BrainiacGlobals::RX:
                //m.rotate()
                m_restMatrix*=osg::Matrix::rotate(BrainiacGlobals::grad2rad(m_oRx->getOldValue()),osg::Vec3d(1.0,0.0,0.0));
                break;
            case BrainiacGlobals::RY:
                m_restMatrix*=osg::Matrix::rotate(BrainiacGlobals::grad2rad(m_oRy->getOldValue()),osg::Vec3d(0.0,1.0,0.0));
                break;
            case BrainiacGlobals::RZ:
                m_restMatrix*=osg::Matrix::rotate(BrainiacGlobals::grad2rad(m_oRz->getOldValue()),osg::Vec3d(0.0,0.0,1.0));
                break;
            case BrainiacGlobals::TX:
                m_restMatrix*=osg::Matrix::translate(m_oTx->getOldValue(),0.0,0.0);
                break;
            case BrainiacGlobals::TY:
                m_restMatrix*=osg::Matrix::translate(0.0,m_oTy->getOldValue(),0.0);
                break;
            case BrainiacGlobals::TZ:
                m_restMatrix*=osg::Matrix::translate(0.0,0.0,m_oTz->getOldValue());
                break;
            }
        }
        m_restMatrixApplied=false;
        //this->setMatrix(m);
        //this->setMatrix(osg::Matrix::translate(25.0, 0.0, 0.0 ));
        m_restMatrixDirty=false;
    }
}

void BodySegment::createChannels()
{
    QString segName(getName().c_str());
    qCDebug(bChannel) << __PRETTY_FUNCTION__ << "creating channels for Segment" << segName;
    m_oRx=m_body->getAgent()->getOrCreateOutputChannel(segName % ":rx");
    QObject::connect(m_oRx,SIGNAL(oldValueChanged(qreal)),m_signalHandler,SLOT(restMatrixChanged()),Qt::DirectConnection);

    m_oRy=m_body->getAgent()->getOrCreateOutputChannel(segName % ":ry");
    QObject::connect(m_oRy,SIGNAL(oldValueChanged(qreal)),m_signalHandler,SLOT(restMatrixChanged()),Qt::DirectConnection);

    m_oRz=m_body->getAgent()->getOrCreateOutputChannel(segName % ":rz");
    QObject::connect(m_oRz,SIGNAL(oldValueChanged(qreal)),m_signalHandler,SLOT(restMatrixChanged()),Qt::DirectConnection);

    m_oTx=m_body->getAgent()->getOrCreateOutputChannel(segName % ":tx");
    QObject::connect(m_oTx,SIGNAL(oldValueChanged(qreal)),m_signalHandler,SLOT(restMatrixChanged()),Qt::DirectConnection);

    m_oTy=m_body->getAgent()->getOrCreateOutputChannel(segName % ":ty");
    QObject::connect(m_oTy,SIGNAL(oldValueChanged(qreal)),m_signalHandler,SLOT(restMatrixChanged()),Qt::DirectConnection);

    m_oTz=m_body->getAgent()->getOrCreateOutputChannel(segName % ":tz");
    QObject::connect(m_oTz,SIGNAL(oldValueChanged(qreal)),m_signalHandler,SLOT(restMatrixChanged()),Qt::DirectConnection);

    m_oColor=m_body->getAgent()->getOrCreateOutputChannel(segName % ":color");
    m_oColor->setDefault(1);
    QObject::connect(m_oColor,SIGNAL(oldValueChanged(qreal)),m_signalHandler,SLOT(colorChanged()),Qt::DirectConnection);

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
    m_oTx->setValue(m_segmentShape->getRestTranslation().x());
    m_oTy->setValue(m_segmentShape->getRestTranslation().y());
    m_oTz->setValue(m_segmentShape->getRestTranslation().z());

    m_oRx->setValue(m_segmentShape->getRestRotation().x());
    m_oRy->setValue(m_segmentShape->getRestRotation().y());
    m_oRz->setValue(m_segmentShape->getRestRotation().z());

    m_oRx->advance();
    m_oRy->advance();
    m_oRx->advance();
    m_oTx->advance();
    m_oTy->advance();
    m_oTz->advance();

    m_restMatrixDirty=true;
    m_restMatrixApplied=false;
    computeRestMatrix();

}

void BodySegment::toggleHighlight()
{
    m_shouldHighlight=!m_shouldHighlight;
}

void BodySegment::traverse(osg::NodeVisitor &nv)
{
    //computeRestMatrix();
    if(!m_restMatrixApplied) {
        this->setMatrix(m_restMatrix);
        m_restMatrixApplied=true;
    }
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
    m_signalHandler->deleteLater();
    m_body->getAgent()->deleteChannel(m_oRx);
    m_body->getAgent()->deleteChannel(m_oRy);
    m_body->getAgent()->deleteChannel(m_oRz);
    m_body->getAgent()->deleteChannel(m_oTx);
    m_body->getAgent()->deleteChannel(m_oTy);
    m_body->getAgent()->deleteChannel(m_oTz);
    m_body->getAgent()->deleteChannel(m_oColor);
}
