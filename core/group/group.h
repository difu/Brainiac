#ifndef GROUP_H
#define GROUP_H

#include <QString>

class QXmlStreamWriter;
class QXmlStreamReader;
class AgentManager;

class Group
{
public:
    Group();
    quint32 getId() { return m_id; }
    qint32 getEditorTranslationX() { return m_editX; }
    qint32 getEditorTranslationY() { return m_editY; }
    QString & getName() {return m_name;}
    QString & getAgentFileName() { return m_agentFileName; }
    void loadConfig(QXmlStreamReader *xml);
    void saveConfig(QXmlStreamWriter *xml);
    void setEditorTranslation(qint32 x, qint32 y);
    void setId(quint32 id) { m_id=id; }
    void setName( QString name );
    void setAgentFileName( QString fileName );
    AgentManager* getAgentManager() { return m_agentManager; }

protected:
    QString m_name;
    QString m_agentFileName;
    quint32 m_id;
    quint32 m_editX, m_editY;
    AgentManager *m_agentManager;
};

#endif // GROUP_H
