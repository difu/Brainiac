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
