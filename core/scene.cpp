// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "scene.h"
#include <QFile>
#include <QVector4D>
#include "core/simulation.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "generator/generator.h"
#include "generator/pointgenerator.h"
#include "group/group.h"
#include "agent/body/body.h"
#include "core/simulationsettings.h"
#include "core/brainiaclogger.h"
#include "core/brainiacerror.h"

Scene::Scene(QObject *parent) :
    QObject(parent)
{
    // add basic camera
    //m_cameras.append(new Camera(this,0,100,200)); //!< \todo handle (delete?) this camera when scene is loaded
    m_simulation=new Simulation(this);
    m_rootNode=new osg::Group;
    m_rootNode.get()->setName("Scene root node");
}


void Scene::addGenerator(Generator *gen)
{
    m_generators.append(gen);
}

void Scene::addGroup(Group *group)
{
    m_groupIdGenerator.registerId(group->getId());
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
    m_agents.clear();
}

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
        qCDebug(bScene) << __PRETTY_FUNCTION__ << "Creating agents of Generator " << pGen->getName();
        gen->generateLocators();
        foreach(Locator *loc,*pGen->getLocations()) {
            Agent *locAgent=loc->getAgent();
            if(locAgent) {
                continue;
            } else {
                locAgent=loc->createInstance();
                m_agents.append(locAgent); // add the agent to all the other agents of the scene
                m_rootNode.get()->addChild(locAgent->getBody()->getBodyRoot());
            }
        }
    }
    foreach(Group *grp, m_groups) {
        if(grp->getAgentManager()) {
            grp->getAgentManager()->updateSoundConfigs();
        }
    }
}

void Scene::removeAgentFromScene(Agent *agent)
{
    int removedAgents=m_agents.removeAll(agent);
    bool removedFromSceneGraph=m_rootNode.get()->removeChild(agent->getBody()->getBodyRoot());
    if(removedAgents>1) {
        qCWarning(bScene) << __PRETTY_FUNCTION__ << "more than one agent removed from scene list!";
    }
    if(removedAgents>=1 && !removedFromSceneGraph) {
        qCWarning(bScene) << __PRETTY_FUNCTION__ << "could not delete/find agent from scene graph!";
    }
}

QString Scene::getAbsoluteFileDir() const {
    QFileInfo fInfo(m_fileName);
    return fInfo.absolutePath();
}

QString Scene::getRelativeFileDir(const QString &file) const
{
    QDir dir(getAbsoluteFileDir());
    return dir.relativeFilePath(file);
}

QList<Agent *> Scene::getAgents()
{
    return m_agents;
}

QList<Camera *> Scene::getCameras()
{
    return m_cameras;
}

QList<Generator *> Scene::getGenerators() const
{
    return m_generators;
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

QString Scene::getSceneXml() const
{
    QString xml;
    QXmlStreamWriter sWriter(&xml);
    sWriter.setAutoFormatting(true);
    sWriter.writeStartDocument();
    sWriter.writeStartElement(BrainiacGlobals::XmlSceneTag);
    m_simulation->getSettings()->saveConfig(&sWriter);
    sWriter.writeStartElement(BrainiacGlobals::XmlPlaceTag);
    sWriter.writeStartElement(BrainiacGlobals::XmlGroupsTag);
    foreach(Group *grp, m_groups) {
        grp->saveConfig(&sWriter);
    }
    sWriter.writeEndElement(); // Groups
    sWriter.writeStartElement("Generators");
    foreach(Generator *gen, m_generators) {
        if(gen->getType()==Generator::POINT) {
            static_cast<PointGenerator *>(gen)->saveConfig(&sWriter);
        }
    }
    sWriter.writeEndElement(); // Generators
    sWriter.writeEndElement(); // Place
    sWriter.writeEndElement(); // BrainiacScene

    sWriter.writeEndDocument();
    return xml;
}

bool Scene::openConfig(const QString & fileName)
{
    m_fileName=fileName;
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly)) {
        this->clear();
        QXmlStreamReader streamReader;
        streamReader.setDevice(&file);
        bool ok=setByXml(streamReader);
        file.close();
        if(ok)
            createAgents();
        return ok;
    }
    BrainiacError::setLastError(BrainiacError::FILE_NOT_FOUND,__PRETTY_FUNCTION__,0,QString("Failed to open file ").append(fileName));
    return false;
}

bool Scene::saveConfig()
{
    return this->saveConfig(this->m_fileName);
}

bool Scene::saveConfig(const QString & fileName)
{
    QSaveFile file(fileName);
    file.setDirectWriteFallback(true);
    if(file.open(QIODevice::WriteOnly)) {
        file.write( getSceneXml().toStdString().c_str() );
        m_fileName=fileName;
        if(file.commit()) {
            return true;
        } else {
            BrainiacError::setLastError(BrainiacError::COULD_NOT_WRITE_FILE,__PRETTY_FUNCTION__,0,"Failed to write file savely.");
            return false;
        }
    }
    BrainiacError::setLastError(BrainiacError::COULD_NOT_WRITE_FILE,__PRETTY_FUNCTION__,0,"Unable to open file for writing.");
    return false;
}

bool Scene::setByXml(QXmlStreamReader &xmlReader)
{
    clear();
    while(xmlReader.readNextStartElement())  {
        if(xmlReader.name()==BrainiacGlobals::XmlSceneTag) {
            while(xmlReader.readNextStartElement()) {
                if(xmlReader.name()==BrainiacGlobals::XmlPlaceTag) {
                    while(xmlReader.readNextStartElement()) {
                        if(xmlReader.name()=="Generators") {
                            while(xmlReader.readNextStartElement()) {
                                if(xmlReader.name()=="PointGenerator") {
                                    //qDebug() << "scene.cpp openconfig openConfigPointgenerator missing!";
                                    PointGenerator *pg=new PointGenerator(this);
                                    pg->loadConfig(&xmlReader);
                                    m_generators.append(pg);
                                } else {
                                    xmlReader.skipCurrentElement();
                                }
                            } // all generators done
                        } else if(xmlReader.name()==BrainiacGlobals::XmlGroupsTag) {
                            //m_streamReader.skipCurrentElement();
                            while(xmlReader.readNextStartElement()) {
                                if(xmlReader.name()=="Group") {
                                    Group *grp=new Group(this);
                                    grp->loadConfig(&xmlReader);
                                    //addGroup(grp);
                                    //m_groups.append(grp);
                                } else {
                                    xmlReader.skipCurrentElement();
                                }

                            }
                        } else {
                            xmlReader.skipCurrentElement();
                        }
                    }
                    xmlReader.skipCurrentElement();
                } else if(xmlReader.name()==BrainiacGlobals::XmlSimulationTag) {
                    m_simulation->getSettings()->loadConfig(&xmlReader);
                } else {
                    xmlReader.skipCurrentElement();
                }
            }
        }
    }
    return true;
}

void Scene::setShowAgentAxis(bool show)
{
    foreach(Agent *agent, m_agents) {
        agent->getBody()->showPositionMarker(show);
    }
}

bool Scene::setByXml(const QString &xml)
{
    QXmlStreamReader xmlReader(xml);
    return setByXml(xmlReader);
}

Scene::~Scene()
{
    clear();
}
