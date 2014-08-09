// Copyright (C) 2014  Dirk Fuchs dirkfux@googlemail.com

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

#include "segmentsphere.h"

SegmentSphere::SegmentSphere(const SegmentShape &other ):SegmentShape(other)
{
    m_shape= new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f),
                             getDiameter()/2.0);
}

void SegmentSphere::updateAndNotify()
{
    osg::Sphere *sphere;
    sphere=dynamic_cast<osg::Sphere *>(m_shape.get());
    if(sphere) {
        sphere->setRadius(getDiameter()/2.0);
    }
    Segment::updateAndNotify();
}
