#include "generator.h"
#include <QVector4D>

Generator::Generator(GeneratorType genType)
{
    m_genType=genType;
}

QList <Generator::locator>* Generator::getLocations()
{
    return &m_locations;
}

Generator::~Generator()
{
    foreach(locator loc, m_locations) {
        delete loc.locator;
    }
    m_locations.clear();
}
