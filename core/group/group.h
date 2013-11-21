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


#ifndef GROUP_H
#define GROUP_H

#include <QList>
#include <QString>

class QXmlStreamWriter;
class QXmlStreamReader;
class Agent;
class AgentManager;
class Scene;

/**
 * @brief Manages a group
 *
 * A Group is a collection of Agents of the same kind. Internally they are managed by the AgentManager
 *
 * @sa AgentManager
 * @class Group group.h "core/group/group.h"
 */
class Group
{
public:
    Group(Scene *scene);

    /**
     * @brief Creates a new Agent instance with given Id
     *
     * @param id the id of the new Agent
     * @return Agent* a pointer to the newly generated Agent
     */
    Agent* createAndAddNewAgent(quint32 id);

    /**
     * @brief Creates a new agent instance
     *
     * @return Agent* a pointer to the newly generated Agent
     */
    Agent* createAndAddNewAgent();

    /**
     * @brief returns a list of all agents of this Group
     *
     * @fn getAgents
     * @return QList<Agent *>
     */
    QList <Agent *> getAgents() {return m_agents;}

    /**
     * @brief returns the id of this group
     *
     * @fn getId
     * @return quint32
     */
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
    ~Group();

protected:
    QString m_name;
    QString m_agentFileName;
    QString m_agentFileNameAbsolute;
    quint32 m_id;
    quint32 m_editX, /**< x position of group item in SceneEditor */
            m_editY; /**< y position of group item in SceneEditor */
    AgentManager *m_agentManager;
    Scene *m_scene;
    QList <Agent *> m_agents; /**< all agents of this group */
};

#endif // GROUP_H
