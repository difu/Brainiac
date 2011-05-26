#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class Agent;
class Camera;
class Generator;
class Group;

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    void addAgent(Agent *agent);
    void addGroup(Group *group);
    void clear(); //!< deletes all contents from this scene
    QList<Agent *> getAgents();
    QList<Camera *> getCameras();
    Group *getGroup(quint32 id);
    QList<Group *> getGroups() { return m_groups; }
    bool openConfig(const QString & fileName);
    bool saveConfig(const QString & fileName);
    bool saveConfig();

protected:
    void createAgents();
    void createAgents(Generator *gen);
    QString m_fileName;
    QXmlStreamWriter m_streamWriter;
    QXmlStreamReader m_streamReader;
    QList<Agent *> m_agents; //!< all agents of this scene
    QList<Camera *> m_cameras;
    QList<Generator *> m_generators;
    QList<Group *> m_groups;

signals:
    void groupAdded(Group *group);

public slots:

};

#endif // SCENE_H
