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


#ifndef LOCATOR_H
#define LOCATOR_H

#include <QObject>
#include <QtGlobal>
#include <QVector4D>

class Group;
class Agent;
class Generator;

/**
 * @brief Describes an agent position in the Scene
 *
 */
class Locator : public QObject
{
    Q_OBJECT
public:

/**
 * @brief Constructs a new Locator
 *
 * @param group the group
 * @param x x ordinate
 * @param y y ordinate
 * @param z z ordinate
 * @param w rotation angle around y axis
 */
    Locator(Group *group, Generator *generator, qreal x=0.0, qreal y=0.0, qreal z=0.0, qreal w=0.0);

    Group* getGroup() {return m_group;}

    /**
     * @brief returns the agent this Locater is associated with
     * @return Agent * pointer to the agent
     */
    Agent* getAgent();

    /**
     * @brief returns the xyz position and rotation angle around y axis
     *
     * @return QVector4D position and y rotation
     */
    QVector4D getLocation() const;

    QVector4D& getLocationRelativeToCenter();

    /**
     * @brief checks, if this Locator has an instanced agent
     *
     * @fn hasInstance
     * @return bool true, if this locator has an instanced agent, false otherwise
     */
    bool hasInstance() const;

    /**
     * @brief sets the agent to this locator
     *
     * @param Agent *agent pointer to the agent
     */
    void setAgent(Agent *agent);

    /**
     * @brief Destructor
     *
     */

    virtual ~Locator();
private:
    QVector4D m_position; /**< Position and rotation of agent */
    Generator *m_generator; /**< The generator of this locator */
    Group *m_group; /**< Group of this locator */
};

#endif // LOCATOR_H
