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

#include "bodysegmentsignalhandler.h"
#include "core/agent/body/bodysegment.h"

BodySegmentSignalHandler::BodySegmentSignalHandler(BodySegment *bodySegment, QObject *parent) :
    QObject(parent)
{
    m_bodySegment=bodySegment;
    connect(m_bodySegment->getSegmentShape(),SIGNAL(updated()),this,SLOT(matricesChanged()));
}

void BodySegmentSignalHandler::matricesChanged()
{
//    qDebug() << __PRETTY_FUNCTION__ <<"Matrices changed";
    m_bodySegment->computeRestMatrix(true);
    m_bodySegment->computeMatrix();
}
