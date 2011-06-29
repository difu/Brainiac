#ifndef AGENTMANAGER_H
#define AGENTMANAGER_H

#include <QtCore>

class Agent;
class Group;
class QXmlStreamReader;
class Scene;

class AgentManager
{
public:
    AgentManager(Scene *scene, Group *group);
    Agent *cloneAgent(quint32 id);
    quint32 getId() { return m_id; }
    qint32 getEditorTranslationX() { return m_editX; }
    qint32 getEditorTranslationY() { return m_editY; }
    QHash<quint32, QPoint> getEditorFuzzyLocations();
    Group* getGroup() { return m_group; }
    Agent* getMasterAgent() {return m_masterAgent; }
    QString & getName() {return m_name;}
    bool loadConfig();
    bool saveConfig();
    void setEditorTranslation(qint32 x, qint32 y);
    void setFuzzyEditorTranslation(quint32 id, qint32 x, qint32 y);
    void setFuzzyResult(quint32 id, qreal result);
    void setId(quint32 id) { m_id=id; }
    void setName( QString name ) { m_name=name; }
    void setFileName( QString fileName ) { m_fileName=fileName; }

protected:
    Agent *m_masterAgent;
    QString m_name;
    QString m_fileName;
    quint32 m_id;
    quint32 m_editX, m_editY;
    Group *m_group;
    Scene *m_scene;
    QHash<quint32, QPoint> m_editorFuzzyLocations;

    // Brain stuff
    void addAndFuzz(quint32 id, QString name, QString mode, quint32 editorX, quint32 editorY);
    void addDefuzz(quint32 id, QString name, qreal defuzzValue, bool isElse, quint32 editorX, quint32 editorY);
    void addOrFuzz(quint32 id, QString name, QString mode, quint32 editorX, quint32 editorY);
    void addOutputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max, quint32 editorX, quint32 editorY);
    void addInputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max, quint32 editorX, quint32 editorY);
    void addNoiseFuzz(quint32 id, QString name, qreal rate, quint32 editorX, quint32 editorY);
    void addConnector(quint32 childId, quint32 parentId, bool inverted);

    // Body stuff
    void addSphereFromConfig( QXmlStreamReader *reader, quint32 id, QString name, quint32 parent );

};

#endif // AGENTMANAGER_H
