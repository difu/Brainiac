#include "pointgenerator.h"
#include <QVector4D>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

PointGenerator::PointGenerator():Generator(Generator::POINT)
{
}

void PointGenerator::addLocation(qreal x, qreal y, qreal z, qreal w, unsigned int groupId)
{
    QVector4D *vec=new QVector4D(x,y,z,w);
    Generator::locator loc;
    loc.locator=vec;
    loc.groupId=groupId;
    this->addLocation(loc);
}

void PointGenerator::addLocation(QVector4D *location, unsigned int groupId)
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
            qreal x=attribs.value("x").string()->toDouble();
            qreal y=attribs.value("y").string()->toDouble();
            qreal z=attribs.value("z").string()->toDouble();
            qreal w=attribs.value("w").string()->toDouble();
            unsigned int groupId=attribs.value("group").string()->toInt();
            QVector4D *vec=new QVector4D(x,y,z,w);
            Generator::locator loc;
            loc.locator=vec;
            loc.groupId=groupId;
            this->addLocation(loc);
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
