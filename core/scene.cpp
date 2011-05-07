#include "scene.h"
#include <QFile>
#include "generator/generator.h"
#include "generator/pointgenerator.h"
#include "group/group.h"
#include <QDebug>

Scene::Scene(QObject *parent) :
    QObject(parent)
{
//    PointGenerator *pg=new PointGenerator();
//    pg->addLocation(1,2,3,4,5);
//    m_generators.append(pg);
//    saveConfig("/tmp/blubber.xml");
}

void Scene::addGroup(Group *group)
{
    m_groups.append(group);
    emit groupAdded(group);
}

void Scene::clear()
{
    foreach(Generator *gen, m_generators) {
        delete gen;
    }
    m_generators.clear();

    foreach(Group *grp, m_groups) {
        delete grp;
    }
    m_groups.clear();
}

bool Scene::openConfig(const QString & fileName)
{
    m_fileName=fileName;
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly)) {
        this->clear();
        m_streamReader.setDevice(&file);
        while(m_streamReader.readNextStartElement()) {
            if(m_streamReader.name()=="Place") {
                while(m_streamReader.readNextStartElement()) {
                    if(m_streamReader.name()=="Generators") {
                        while(m_streamReader.readNextStartElement()) {
                            if(m_streamReader.name()=="PointGenerator") {
                                PointGenerator *pg=new PointGenerator();
                                pg->loadConfig(&m_streamReader);
                                m_generators.append(pg);
                            } else {
                                m_streamReader.skipCurrentElement();
                            }
                        } // all generators done
                    } else if(m_streamReader.name()=="Groups") {
                        qDebug() << "Groups";
                        //m_streamReader.skipCurrentElement();
                        while(m_streamReader.readNextStartElement()) {
                            if(m_streamReader.name()=="Group") {
                                Group *grp=new Group();
                                grp->loadConfig(&m_streamReader);
                                addGroup(grp);
                                //m_groups.append(grp);
                            } else {
                                m_streamReader.skipCurrentElement();
                            }

                        }
                    } else {
                        m_streamReader.skipCurrentElement();
                    }
                }
            } else {
                m_streamReader.skipCurrentElement();
            }
        }
        file.close();
        return true;
    }
    return false;
}

bool Scene::saveConfig()
{
    return this->saveConfig(this->m_fileName);
}

bool Scene::saveConfig(const QString & fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadWrite|QIODevice::Truncate)) {
        m_streamWriter.setDevice(&file);
        m_streamWriter.setAutoFormatting(true);
        m_streamWriter.writeStartDocument();
        m_streamWriter.writeStartElement("Place");
        m_streamWriter.writeStartElement("Groups");
        foreach(Group *grp, m_groups) {
            grp->saveConfig(&m_streamWriter);
        }
        m_streamWriter.writeEndElement(); // Groups
        m_streamWriter.writeStartElement("Generators");
        foreach(Generator *gen, m_generators) {
            if(gen->getType()==Generator::POINT) {
                static_cast<PointGenerator *>(gen)->saveConfig(&m_streamWriter);
            }
        }
        m_streamWriter.writeEndElement(); // Generators
        m_streamWriter.writeEndElement(); // Place

        m_streamWriter.writeEndDocument();
        file.close();
        return true;
    }

    return false;
}
