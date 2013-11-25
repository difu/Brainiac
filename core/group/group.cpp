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


#include "group.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFileInfo>
#include <QDir>
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/scene.h"

Group::Group(Scene *scene)
{
    m_scene=scene;
    m_id=m_editX=m_editY=0;
    m_agentManager=new AgentManager(this);
    m_scene->addGroup(this);
}

//void Group::addAgent(Agent *agent)
//{
//    m_agents.append(agent);
//}

Agent* Group::createAndAddNewAgent(quint32 id)
{
    Agent *agent=m_agentManager->createNewAgentInstance(id);
    m_agents.append(agent);
    return agent;
}

Agent* Group::createAndAddNewAgent()
{
    return this->createAndAddNewAgent(this->getNextAgentId());
}

quint32 Group::getNextAgentId() {
    quint32 highestId=0;
    foreach(Agent *agent,m_agents) {
        if(agent->getId()>highestId) {
            highestId=agent->getId();
        }
    }
    return ++highestId;
}

void Group::loadConfig(QXmlStreamReader *xml)
{
    Q_ASSERT(xml->isStartElement() && xml->name() == "Group");
    QXmlStreamAttributes attribs = xml->attributes();
    setId(attribs.value("id").toString().toInt());
    setName(attribs.value("name").toString());
    setAgentFileName(attribs.value("agentFile").toString());
    setEditorTranslation(attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
    m_agentManager->setEditorTranslation(attribs.value("agentEditorX").toString().toInt(),attribs.value("agentEditorY").toString().toInt());
    xml->skipCurrentElement();

    //m_agentManager=new AgentManager(m_scene, this);
    m_agentManager->setFileName(m_agentFileNameAbsolute);
    if(m_agentManager->loadConfig()) {

    } else {
        delete m_agentManager;
        qCritical() << "Error while loading agentFile" << m_agentFileName;
    }
}

void Group::saveConfig(QXmlStreamWriter *xml)
{
    xml->writeStartElement("Group");
    xml->writeAttribute("name",m_name);
    xml->writeAttribute("id",QString::number(m_id));
    xml->writeAttribute("editorx",QString::number(m_editX));
    xml->writeAttribute("editory",QString::number(m_editY));
    xml->writeAttribute("agentEditorX",QString::number(m_agentManager->getEditorTranslationX()));
    xml->writeAttribute("agentEditorY",QString::number(m_agentManager->getEditorTranslationY()));
    xml->writeAttribute("agentFile",m_agentFileName);
    xml->writeEndElement();
}

void::Group::setAgentFileName(QString fileName)
{
    m_agentFileName=fileName;
    QFileInfo fileInfo(fileName);
    if(fileInfo.isAbsolute()) {
        m_agentFileNameAbsolute=fileInfo.absoluteFilePath();
    } else {
        QFileInfo fileInfoScene(m_scene->getFileName());
        QDir path=fileInfoScene.absoluteDir();
        m_agentFileNameAbsolute=path.absolutePath();
        m_agentFileNameAbsolute.append(QDir::separator());
        m_agentFileNameAbsolute.append(fileName);
    }

//    qDebug() << "FileName "<< m_agentFileName;
}

void Group::setEditorTranslation(qint32 x, qint32 y)
{
    m_editX=x;
    m_editY=y;
}

void Group::setName(QString name)
{
    m_name=QString(name);
}

Group::~Group()
{
    m_agents.clear();
    delete(m_agentManager);
}
