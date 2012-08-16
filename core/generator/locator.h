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

#include <QtGlobal>
#include <QVector4D>

class Group;

/**
 * @brief Describes an agent position in the Scene
 *
 */
class Locator
{
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
    Locator(Group *group, qreal x, qreal y, qreal z, qreal w);

    Group* getGroup() const {return m_group;}
    /**
     * @brief returns the xyz position and rotation angle around y axis
     *
     * @return QVector4D * position and rotation
     */
    QVector4D& getLocation();

    /**
     * @brief Destructor
     *
     */

    virtual ~Locator();
protected:
    QVector4D m_position; /**< Position and rotation of agent */
    Group *m_group; /**< Group of this locator */
};

#endif // LOCATOR_H
