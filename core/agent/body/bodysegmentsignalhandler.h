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

#ifndef BODYSEGMENTSIGNALHANDLER_H
#define BODYSEGMENTSIGNALHANDLER_H

#include <QObject>

class BodySegment;

class BodySegmentSignalHandler: public QObject
{
    Q_OBJECT
public:
    explicit BodySegmentSignalHandler(BodySegment *bodySegment, QObject *parent = 0);

signals:

public slots:
    void matricesChanged();

protected:
    BodySegment *m_bodySegment;

};

#endif // BODYSEGMENTSIGNALHANDLER_H
