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
        m_shapeDrawable->setShape( new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f),
                                                1.0f) );
        break;
    default:
        m_shapeDrawable->setShape( new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f),
                                                1.0f, 1.0f, 1.0f) );
        break;

    }
    m_shapeDrawable->setColor( osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f) );
    m_shapeDrawable->setName("ShapeDrawable");
    //qDebug() << __PRETTY_FUNCTION__ << "Shape created" << other.getName();
}

SegmentShape::~SegmentShape()
{
    //delete m_shapeDrawable;
}
