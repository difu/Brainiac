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


#include "locator.h"
#include "core/generator/generator.h"

Locator::Locator(Group *group, Generator *generator, qreal x, qreal y, qreal z, qreal w):
    m_position(QVector4D(x,y,z,w)),
    m_generator(generator),
    m_group(group),
    m_agent(0)
{
}

bool Locator::hasInstance() const
{
    if(m_agent) {
        return true;
    }
    return false;
}

QVector4D Locator::getLocation() const
{
    return QVector4D(m_position.x()+m_generator->getCenter().x(),
                     m_position.y()+m_generator->getCenter().y(),
                     m_position.z()+m_generator->getCenter().z(),
                     m_position.w());
}

QVector4D& Locator::getLocationRelativeToCenter()
{
    return m_position;
}

Locator::~Locator()
{
    //delete m_position;
}
