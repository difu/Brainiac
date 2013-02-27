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
#include <osgUtil/UpdateVisitor>

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
    computeRestMatrix();
    setName(m_segmentShape->getName().toStdString());
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
                m*=osg::Matrix::rotate(BrainiacGlobals::grad2rad(m_segmentShape->getRestRotation().x()),osg::Vec3d(1.0f,0.0f,0.0f));
                break;
            case BrainiacGlobals::RY:
                m*=osg::Matrix::rotate(BrainiacGlobals::grad2rad(m_segmentShape->getRestRotation().y()),osg::Vec3d(0.0f,1.0f,0.0f));
                break;
            case BrainiacGlobals::RZ:
                m*=osg::Matrix::rotate(BrainiacGlobals::grad2rad(m_segmentShape->getRestRotation().z()),osg::Vec3d(0.0f,0.0f,1.0f));
                break;
            case BrainiacGlobals::TX:
                m*=osg::Matrix::translate(m_segmentShape->getRestTranslation().x(),0.0f,0.0f);
                break;
            case BrainiacGlobals::TY:
                m*=osg::Matrix::translate(0.0f,m_segmentShape->getRestTranslation().y(),0.0f);
                break;
            case BrainiacGlobals::TZ:
                m*=osg::Matrix::translate(0.0f,0.0f,m_segmentShape->getRestTranslation().z());
                break;
            }
        }
        this->setMatrix(m);
        //this->setMatrix(osg::Matrix::translate(25.0f, 0.0f, 0.0f ));
        m_restMatrixDirty=false;
    }
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
