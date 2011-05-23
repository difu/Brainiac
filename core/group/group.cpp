#include "group.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/scene.h"

Group::Group(Scene *scene)
{
    m_scene=scene;
    m_id=m_editX=m_editY=0;
}

void Group::addAgent(Agent *agent)
{
    m_agents.append(agent);
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
    xml->skipCurrentElement();

    m_agentManager=new AgentManager(m_scene);
    m_agentManager->setFileName(m_agentFileName);
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
    xml->writeAttribute("agentFile",m_agentFileName);
    xml->writeEndElement();
}

void::Group::setAgentFileName(QString fileName)
{
    m_agentFileName=fileName;
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
