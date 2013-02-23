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

BodySegment::BodySegment(Body *body, SegmentShape *segmentShape):MatrixTransform(),m_body(body),m_segmentShape(segmentShape)
{
    m_channelHandler=new BodySegmentSignalHandler(this);
    addChild(m_segmentShape->getGLSegment());
    m_matrixDirty=true;
    computeMatrix();
    setName(m_segmentShape->getName().toStdString());
}

void BodySegment::computeMatrix() {
    if(m_matrixDirty) {
        osg::Matrix m;
        foreach(BrainiacGlobals::RotTrans rotTrans,m_segmentShape->getRotationTranslationOrder()) {
            switch(rotTrans) {
            case BrainiacGlobals::RX:
                //m.rotate()
                m*=osg::Matrix::rotate(m_segmentShape->getRestRotation().x(),osg::Vec3d(1.0f,0.0f,0.0f));
                break;
            case BrainiacGlobals::RY:
                m*=osg::Matrix::rotate(m_segmentShape->getRestRotation().y(),osg::Vec3d(0.0f,1.0f,0.0f));
                break;
            case BrainiacGlobals::RZ:
                m*=osg::Matrix::rotate(m_segmentShape->getRestRotation().z(),osg::Vec3d(0.0f,0.0f,1.0f));
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
        m_matrixDirty=false;
    }
}

BodySegment::~BodySegment()
{
    m_channelHandler->deleteLater();
}
