#include "scene.h"
#include <QFile>
#include <QVector4D>
#include "core/simulation.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "generator/generator.h"
#include "generator/pointgenerator.h"
#include "group/group.h"
#include "camera.h"
#include <QDebug>

Scene::Scene(QObject *parent) :
    QObject(parent)
{
    // add basic camera
    m_cameras.append(new Camera(this,0,100,200)); //!< \todo handle (delete?) this camera when scene is loaded
    m_simulation=new Simulation(this);
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


/** \brief creates all agents of this scene



**/
void Scene::createAgents()
{
    foreach(Generator *gen, m_generators) {
        this->createAgents(gen);
    }
}

void Scene::createAgents(Generator *gen)
{
    if(gen->getType()==Generator::POINT) {
        PointGenerator *pGen=(PointGenerator *)gen;
        gen->generateLocators();
        Group *bla=pGen->getLocations()->at(0)->getGroup();
        foreach(Locator *loc,*pGen->getLocations()) {
            Group *grp=loc->getGroup();
            QVector4D trans=loc->getLocation();
            if(grp->getAgentManager()) { // only, if we successfully loaded an agent
                Agent *agent=grp->getAgentManager()->cloneAgent(grp->getNextAgentId());
                agent->setRestTranslation(trans.x(),trans.y(),trans.z()); //!< \todo add rest rotation
                agent->setRestRotation(0,trans.w(),0);
                m_agents.append(agent);
                grp->addAgent(agent);
                agent->reset();
            }
        }
    }
    foreach(Group *grp, m_groups) {
        if(grp->getAgentManager()) {
            grp->getAgentManager()->updateSoundConfigs();
        }
    }
}

QList<Agent *> Scene::getAgents()
{
    return m_agents;
}

QList<Camera *> Scene::getCameras()
{
    return m_cameras;
}

/** \brief get group by its id

        @returns pointer to the group with given id
        @returns 0 if no group is found
**/
Group* Scene::getGroup(quint32 id)
{
    foreach(Group *grp,m_groups) {
        if(grp->getId()==id) {
            return grp;
        }
    }
    return 0;
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
                                //qDebug() << "scene.cpp openconfig openConfigPointgenerator missing!";
                                PointGenerator *pg=new PointGenerator(this);
                                pg->loadConfig(&m_streamReader);
                                m_generators.append(pg);
                            } else {
                                m_streamReader.skipCurrentElement();
                            }
                        } // all generators done
                    } else if(m_streamReader.name()=="Groups") {
                        //m_streamReader.skipCurrentElement();
                        while(m_streamReader.readNextStartElement()) {
                            if(m_streamReader.name()=="Group") {
                                Group *grp=new Group(this);
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
        createAgents();
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
