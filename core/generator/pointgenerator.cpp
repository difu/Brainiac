#include "pointgenerator.h"
#include <QVector4D>
#include <QVector3D>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>
#include "core/brainiacglobals.h"
#include "core/scene.h"

PointGenerator::PointGenerator(Scene *scene):Generator(Generator::POINT, scene)
{
}

void PointGenerator::addLocation(qreal x, qreal y, qreal z, qreal w, Group *group)
{
    Locator* loc=new Locator(group, x, y, z, w);
    m_locations.append(loc);
}

void PointGenerator::generateLocators()
{
    static qreal min_rad=1.0f;
    QHashIterator<quint32,qreal> groupRatioIt(m_groupRatio);
    Q_ASSERT(groupRatioIt.hasNext());
    groupRatioIt.next();
    quint32 groupId=groupRatioIt.key();
    quint32 higherGIndex=groupRatioIt.value()*m_number;
    for(quint32 i=0; i<m_number;i++) {
        if(i>higherGIndex) {
            if(groupRatioIt.hasNext()) {
                groupRatioIt.next();
                higherGIndex=groupRatioIt.value()*m_number;
                groupId=groupRatioIt.key();
            } else {
                qWarning() << __PRETTY_FUNCTION__ << "There should be a next group Id and a new higher bound"<<i<<higherGIndex;
            }
        }

        qreal angle=2*BrainiacGlobals::PI*BrainiacGlobals::getRand(i+m_id*3+13);
        qreal x=cos(angle)*min_rad;
        qreal z=sin(angle)*min_rad;
        qreal y=0;
        qreal yOrient=m_angle+m_angleNoise*BrainiacGlobals::getRand(i+m_id*3)-m_angleNoise/2.0f;
        Locator *newLoc=new Locator(m_scene->getGroup(groupId),x+m_centerPos.x(),y,z+m_centerPos.z(),yOrient);
        //quint32 i=1;
        quint32 j=1; // Correction step
        bool allOk=false;
        while(!allOk) {
            allOk=true;

            foreach(Locator *loc,m_locations) {
                QVector3D vec=loc->getLocation().toVector3D()-newLoc->getLocation().toVector3D();
                qreal dist=vec.length();

                while(dist<m_distance) {
                    allOk=false;
                    x=cos(angle+j)*min_rad;
                    z=sin(angle+j)*min_rad;
                    qreal x1=x*1.1f*j+m_centerPos.x();
                    qreal z1=z*1.1f*j+m_centerPos.z();
                    newLoc->getLocation().setX(x1);
                    newLoc->getLocation().setZ(z1);
                    vec=loc->getLocation().toVector3D()-newLoc->getLocation().toVector3D();
                    dist=vec.length();
                    j++;
                    //qDebug() << "Correction" << m_id << i << angle << newLoc->getLocation();
                }
                if(!allOk) // if there was a correction start from the beginning, as there might be items already correctly checked and now collide
                    break;

            }
        }
        m_locations.append(newLoc);
    }
}

void PointGenerator::loadConfig(QXmlStreamReader *xml)
{
    Q_ASSERT(xml->isStartElement() && xml->name() == "PointGenerator");
//    while (xml->readNextStartElement()) {
//        if(xml->name()=="Point") {
//            QXmlStreamAttributes attribs = xml->attributes();
//            qreal x=attribs.value("x").toString().toDouble();
//            qreal y=attribs.value("y").toString().toDouble();
//            qreal z=attribs.value("z").toString().toDouble();
//            qreal w=attribs.value("w").toString().toDouble();
//            quint32 groupId=attribs.value("group").toString().toInt();
//            QVector4D *vec=new QVector4D(x,y,z,w);
//            Generator::locator loc;
//            loc.locator=vec;
//            loc.groupId=groupId;
//            this->addLocation(loc);
//            xml->skipCurrentElement();
//        }
//    }
    QXmlStreamAttributes attribs = xml->attributes();
    readCommonXmlAttribs(attribs);

    xml->skipCurrentElement();

}

void PointGenerator::saveConfig(QXmlStreamWriter *xml)
{
    xml->writeStartElement("PointGenerator");
//    foreach(locator loc,m_locations) {
//        xml->writeStartElement("Point");
//        xml->writeAttribute("x",QString::number(loc.locator->x()));
//        xml->writeAttribute("y",QString::number(loc.locator->y()));
//        xml->writeAttribute("z",QString::number(loc.locator->z()));
//        xml->writeAttribute("w",QString::number(loc.locator->w()));
//        xml->writeAttribute("group",QString::number(loc.groupId));
//        xml->writeEndElement();
//    }
    xml->writeEndElement();
}
