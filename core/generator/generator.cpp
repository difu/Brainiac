#include "generator.h"
#include <QVector4D>
#include <QXmlStreamAttributes>
#include "core/brainiacglobals.h"

Generator::Generator(GeneratorType genType,Scene *scene):m_genType(genType),m_scene(scene),m_id(0)
{

}

const QList <Locator *>* Generator::getLocations() const
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
