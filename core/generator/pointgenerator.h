#ifndef POINTGENERATOR_H
#define POINTGENERATOR_H

#include "generator.h"

class QXmlStreamWriter;
class QXmlStreamReader;

class PointGenerator : public Generator
{
public:
    PointGenerator();
    void addLocation(qreal x, qreal y, qreal z, qreal w, quint32 groupId);
    void addLocation(QVector4D *location, quint32 groupId);
    void addLocation(locator loc);
    void loadConfig(QXmlStreamReader *xml);
    void saveConfig(QXmlStreamWriter *xml);
};

#endif // POINTGENERATOR_H
