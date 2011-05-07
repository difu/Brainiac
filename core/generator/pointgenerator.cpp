#include "pointgenerator.h"
#include <QVector4D>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>

PointGenerator::PointGenerator():Generator(Generator::POINT)
{
}

void PointGenerator::addLocation(qreal x, qreal y, qreal z, qreal w, quint32 groupId)
{
    QVector4D *vec=new QVector4D(x,y,z,w);
    Generator::locator loc;
    loc.locator=vec;
    loc.groupId=groupId;
    this->addLocation(loc);
}

void PointGenerator::addLocation(QVector4D *location, quint32 groupId)
{
    Generator::locator loc;
    loc.locator=location;
    loc.groupId=groupId;
    this->addLocation(loc);
}

void PointGenerator::addLocation(locator loc)
{
    m_locations.append(loc);
}

void PointGenerator::loadConfig(QXmlStreamReader *xml)
{
    Q_ASSERT(xml->isStartElement() && xml->name() == "PointGenerator");
    while (xml->readNextStartElement()) {
        if(xml->name()=="Point") {
            QXmlStreamAttributes attribs = xml->attributes();
            qreal x=attribs.value("x").toString().toDouble();
            qreal y=attribs.value("y").toString().toDouble();
            qreal z=attribs.value("z").toString().toDouble();
            qreal w=attribs.value("w").toString().toDouble();
            quint32 groupId=attribs.value("group").toString().toInt();
            QVector4D *vec=new QVector4D(x,y,z,w);
            Generator::locator loc;
            loc.locator=vec;
            loc.groupId=groupId;
            this->addLocation(loc);
            xml->skipCurrentElement();
        }
    }

}

void PointGenerator::saveConfig(QXmlStreamWriter *xml)
{
    xml->writeStartElement("PointGenerator");
    foreach(locator loc,m_locations) {
        xml->writeStartElement("Point");
        xml->writeAttribute("x",QString::number(loc.locator->x()));
        xml->writeAttribute("y",QString::number(loc.locator->y()));
        xml->writeAttribute("z",QString::number(loc.locator->z()));
        xml->writeAttribute("w",QString::number(loc.locator->w()));
        xml->writeAttribute("group",QString::number(loc.groupId));
        xml->writeEndElement();
    }
    xml->writeEndElement();
}
