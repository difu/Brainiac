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

#include "simulationsettings.h"
#include "core/brainiacglobals.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

SimulationSettings::SimulationSettings(QObject *parent) :
    QObject(parent)
{
    m_writeMotion=false;
    m_fps=24;
}

void SimulationSettings::loadConfig(QXmlStreamReader *xml)
{
    Q_ASSERT(xml->isStartElement() && xml->name() == BrainiacGlobals::XmlSimulationTag);
    QXmlStreamAttributes attribs = xml->attributes();
    m_fps=attribs.value(BrainiacGlobals::XmlFpsAttrib).toString().toInt();

    while (xml->readNextStartElement()) {
        if(xml->isStartElement() && xml->name() == BrainiacGlobals::XmlSimulationDirectoryOutputTag) {
            QXmlStreamAttributes attribs = xml->attributes();
            m_motionOutDir=attribs.value(BrainiacGlobals::XmlNameAttrib).toString();
            m_writeMotion=attribs.value(BrainiacGlobals::XmlEnabledAttrib).toString().toInt()==1;
            xml->skipCurrentElement();
        } else {
            xml->skipCurrentElement();
        }
    }
    //xml->skipCurrentElement();
}


void SimulationSettings::saveConfig(QXmlStreamWriter *xml)
{
    xml->writeStartElement(BrainiacGlobals::XmlSimulationTag);
    xml->writeAttribute(BrainiacGlobals::XmlFpsAttrib,QString::number(m_fps));
    xml->writeStartElement(BrainiacGlobals::XmlSimulationDirectoryOutputTag);
    xml->writeAttribute(BrainiacGlobals::XmlNameAttrib,m_motionOutDir);
    xml->writeAttribute(BrainiacGlobals::XmlEnabledAttrib,QString::number((int)m_writeMotion));
    xml->writeEndElement();
    xml->writeEndElement();
}
