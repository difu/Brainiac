#ifndef GROUP_H
#define GROUP_H

#include <QList>
#include <QString>

class QXmlStreamWriter;
class QXmlStreamReader;
class Agent;
class AgentManager;
class Scene;

class Group
{
public:
    Group(Scene *scene);
    void addAgent(Agent *agent);
    QList <Agent *> getAgents() {return m_agents;}
    quint32 getId() { return m_id; }
    qint32 getEditorTranslationX() { return m_editX; }
    qint32 getEditorTranslationY() { return m_editY; }
    QString & getName() {return m_name;}
    quint32 getNextAgentId();
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
    Scene *m_scene;
    QList <Agent *> m_agents;
};

#endif // GROUP_H
