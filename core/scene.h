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

    /**
     * @brief sets the file name of the scene
     *
     * Invoking this function does not save the scene!
     *
     * @fn setFileName
     * @param fileName
     */
    void setFileName(const QString &fileName) { m_fileName=fileName; }

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
