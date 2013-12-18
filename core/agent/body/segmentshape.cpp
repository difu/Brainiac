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

#include "segmentshape.h"

SegmentShape::SegmentShape(const Segment &other ):Segment(other)
{
    m_shapeDrawable=new osg::ShapeDrawable;
    m_transformNode.get()->setName(getName().toStdString());
    switch(m_segmentType) {
    case BrainiacGlobals::SPHERESEGMENT:
        m_shape=new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f),
                                1.0f);
        break;

//    case BrainiacGlobals::TUBESEGMENT:
//        m_shape= new osg::Capsule(osg::Vec3(0.0f, 0.0f, 0.0f),1,1);
//        break;
    default:
        break;
    }
    //m_shapeDrawable->setShape( m_shape );
    m_shapeDrawable->setColor( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    m_shapeDrawable->setName("ShapeDrawable");
    //qDebug() << __PRETTY_FUNCTION__ << "Shape created" << other.getName();
}

void SegmentShape::updateAndNotify()
{
    osg::Sphere *sp;
    osg::Capsule *cap;
    osg::Box *box;
    switch(m_segmentType) {
    case BrainiacGlobals::SPHERESEGMENT:
        sp=dynamic_cast<osg::Sphere *>(m_shape.release());
        break;

    case BrainiacGlobals::TUBESEGMENT:
        cap=dynamic_cast<osg::Capsule *>(m_shape.get());
        if(cap) {
            cap->setHeight(getLength());
            cap->setRadius(getDiameter()/2);
        }
        break;
    case BrainiacGlobals::BOXSEGMENT:
        box=dynamic_cast<osg::Box *>(m_shape.get());
        if(box) {
            box->setHalfLengths(osg::Vec3d(m_segmentSize.x()/2,m_segmentSize.y()/2,m_segmentSize.z()/2));
        }
        break;
    default:
        //box=dynamic_cast<osg::Box *>(m_shape.release());
        break;

    }
    m_shapeDrawable->dirtyDisplayList();
    Segment::updateAndNotify();
}

SegmentShape::~SegmentShape()
{
    //delete m_shapeDrawable;
}
