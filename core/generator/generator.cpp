#include "generator.h"
#include <QVector4D>

Generator::Generator(GeneratorType genType)
{
    m_genType=genType;
}

QList <Generator::locator> Generator::getLocations()
{
    return m_locations;
}
