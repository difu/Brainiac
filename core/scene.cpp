#include "scene.h"
#include <QFile>
#include "generator/generator.h"
#include "generator/pointgenerator.h"

Scene::Scene(QObject *parent) :
    QObject(parent)
{
//    PointGenerator *pg=new PointGenerator();
//    pg->addLocation(1,2,3,4,5);
//    m_generators.append(pg);
//    saveConfig("/tmp/blubber.xml");
}


bool Scene::openConfig(const QString & fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly)) {
        file.close();
        return true;
    }
    return false;
}


bool Scene::saveConfig(const QString & fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite|QIODevice::Truncate)) {
        m_streamWriter.setDevice(&file);
        m_streamWriter.setAutoFormatting(true);
        m_streamWriter.writeStartDocument();
        foreach(Generator *gen, m_generators) {
            if(gen->getType()==Generator::POINT) {
                static_cast<PointGenerator *>(gen)->saveConfig(&m_streamWriter);
            }
        }

        m_streamWriter.writeEndDocument();
        file.close();
        return true;
    }

    return false;
}
