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


#ifndef POINTGENERATOR_H
#define POINTGENERATOR_H

#include "generator.h"

class QXmlStreamWriter;
class QXmlStreamReader;

class PointGenerator : public Generator
{
public:
    PointGenerator(Scene *scene);

    void generateLocators();
    void loadConfig(QXmlStreamReader *xml);
    void saveConfig(QXmlStreamWriter *xml);

protected:
    void addLocation(qreal x, qreal y, qreal z, qreal w, Group *group);
};

#endif // POINTGENERATOR_H
