#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QDir>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "core/idgenerator.h"

class Agent;
class Camera;
class Generator;
class Group;
class Simulation;

/** \brief  The Scene

        This class holds all contents of the scene.
        All agents, lights, groups are referenced here

**/
class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    void addGroup(Group *group);
    void clear(); //!< deletes all contents from this scene
    QList<Agent *> getAgents();
    QList<Camera *> getCameras();
    Group *getGroup(quint32 id);
    QList<Group *> getGroups() { return m_groups; }
    QString getFileName() {return m_fileName; }

    /**
     * @brief returns the absolute directory of the scene file
     *
     * @fn getAbsoluteFileDir
     * @return QString
     */
    QString getAbsoluteFileDir() const;

    /**
     * @brief returns the relative path of given file to the location of the scene file
     *
     * all files are saved relative to the scenefile
     * @fn getRelativeFileDir
     * @param file
     * @return QString
     */
    QString getRelativeFileDir(QString file) const;
    Simulation *getSimulation() { return m_simulation; }
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
    Simulation *m_simulation; //!< simulation
    IdGenerator m_groupIdGenerator;

signals:
    void groupAdded(Group *group); //!< emitted when a group is added

public slots:

};

#endif // SCENE_H
