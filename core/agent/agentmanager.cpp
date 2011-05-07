#include "agentmanager.h"
#include <QFile>
#include <QXmlStreamReader>

AgentManager::AgentManager()
{
    m_id=0;
}

bool AgentManager::loadConfig()
{
    QFile file(m_fileName);
    if(file.open(QIODevice::ReadOnly)) {
        QXmlStreamReader reader;
        reader.setDevice(&file);
        while(reader.readNextStartElement()) {
            if(reader.name()=="Agentconfig") {
                while(reader.readNextStartElement()) {
                    if(reader.name()=="Agent") {
                        QXmlStreamAttributes attribs = reader.attributes();
                        setEditorTranslation(attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                        setId(attribs.value("id").toString().toInt());
                        setName(attribs.value("name").toString());
                    } else {
                        reader.skipCurrentElement();
                    }
                }
            } else {
                reader.skipCurrentElement();
            }
        }
        return true;
    }
    return false;
}

bool AgentManager::saveConfig()
{
    return false;
}

void AgentManager::setEditorTranslation(qint32 x, qint32 y)
{
    m_editX=x;
    m_editY=y;
}
