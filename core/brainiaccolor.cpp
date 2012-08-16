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


#include "brainiaccolor.h"

BrainiacColor::BrainiacColor()
{
    setColor(0);
}

BrainiacColor::BrainiacColor(qreal color)
{
    setColor(color);
}

void BrainiacColor::setColor(qreal color)
{
    m_color=qBound((qreal)0,color,(qreal)1);
}

qreal BrainiacColor::getColor()
{
    return m_color;
}

QColor BrainiacColor::getQColor()
{
    return QColor::fromHsvF(m_color,1,1);
}
