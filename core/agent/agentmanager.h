#ifndef AGENTMANAGER_H
#define AGENTMANAGER_H

#include <QtCore>

class Agent;
class QXmlStreamReader;
class Scene;

class AgentManager
{
public:
    AgentManager(Scene *scene);
    Agent *cloneAgent(quint32 id);
    quint32 getId() { return m_id; }
    qint32 getEditorTranslationX() { return m_editX; }
    qint32 getEditorTranslationY() { return m_editY; }
    QString & getName() {return m_name;}
    bool loadConfig();
    bool saveConfig();
    void setEditorTranslation(qint32 x, qint32 y);
    void setId(quint32 id) { m_id=id; }
    void setName( QString name ) { m_name=name; }
    void setFileName( QString fileName ) { m_fileName=fileName; }

protected:
    Agent *m_masterAgent;
    QString m_name;
    QString m_fileName;
    quint32 m_id;
    quint32 m_editX, m_editY;
    Scene *m_scene;

    void addSphereFromConfig( QXmlStreamReader *reader, quint32 id, QString name, quint32 parent );
};

#endif // AGENTMANAGER_H