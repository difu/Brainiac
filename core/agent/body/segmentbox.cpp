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

#include "segmentbox.h"

SegmentBox::SegmentBox(const SegmentShape &other ):SegmentShape(other)
{
    m_shape= new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f),m_segmentSize.x(), m_segmentSize.y(), m_segmentSize.z());
    m_shapeDrawable->setShape(m_shape);
}
