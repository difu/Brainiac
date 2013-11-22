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


#include "generator.h"
#include <QVector4D>
#include <QXmlStreamAttributes>
#include "core/brainiacglobals.h"

Generator::Generator(GeneratorType genType,Scene *scene):m_genType(genType),m_scene(scene),m_id(0)
{
    m_angle=0;
    m_angleNoise=0;
    m_distance=1.0;
    m_number=0;
}

const QList<Locator *> *Generator::getLocations() const
{
    return &m_locations;
}

void Generator::readCommonXmlAttribs(const QXmlStreamAttributes &attribs)
{
    m_id=attribs.value(BrainiacGlobals::XmlIdAttrib).toString().toUInt();
    m_centerPos.setX(attribs.value(BrainiacGlobals::XmlXAttrib).toString().toDouble());
    m_centerPos.setY(attribs.value(BrainiacGlobals::XmlHeightAttrib).toString().toDouble());
    m_centerPos.setZ(attribs.value(BrainiacGlobals::XmlZAttrib).toString().toDouble());
    m_centerPos.setW(attribs.value(BrainiacGlobals::XmlAngleAttrib).toString().toDouble());
    m_number=attribs.value(BrainiacGlobals::XmlNumberAttrib).toString().toUInt();
    m_distance=attribs.value(BrainiacGlobals::XmlDistanceAttrib).toString().toDouble();
    m_angleNoise=attribs.value(BrainiacGlobals::XmlAngleVarAttrib).toString().toDouble();
    m_angle=attribs.value(BrainiacGlobals::XmlAngleAttrib).toString().toDouble();
    QStringList groups=attribs.value(BrainiacGlobals::XmlGroupsAttrib).toString().split(",");
    QStringList groupRation=attribs.value(BrainiacGlobals::XmlGroupRatioAttrib).toString().split(",");
    int i=0;
    foreach(QString grpId,groups) {
        m_groupRatio.insert(grpId.toUInt(),groupRation.at(i).toFloat());
        i++;
    }

    qDebug() << "GEN Id"<< m_id << m_centerPos << m_groupRatio;
}

Generator::~Generator()
{
    m_locations.clear();
}
