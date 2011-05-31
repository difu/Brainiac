#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class Agent;
class Camera;
class Generator;
class Group;

/** \brief  The Scene

        This class holds all contents of the scene.
        All agents, lights, groups are referenced here

**/
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
    QString m_fileName; //!< the filename of this scene´s config file
    QXmlStreamWriter m_streamWriter;
    QXmlStreamReader m_streamReader;
    QList<Agent *> m_agents; //!< all agents of this scene
    QList<Camera *> m_cameras; //!< all cameras of this scene
    QList<Generator *> m_generators; //!< all agent generators of this scene
    QList<Group *> m_groups; //!< all groups of this scene

signals:
    void groupAdded(Group *group); //!< emitted when a group is added

public slots:

};

#endif // SCENE_H
