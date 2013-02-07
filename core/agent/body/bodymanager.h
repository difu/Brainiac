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

#ifndef BODYMANAGER_H
#define BODYMANAGER_H

#include "core/brainiacglobals.h"
#include "core/agent/body/segment.h"
#include "core/agent/body/segmentshape.h"
#include <QVector3D>
#include <QString>

class AgentManager;

/**
 * @brief
 *
 * @class BodyManager bodymanager.h "core/agent/body/bodymanager.h"
 */
class BodyManager
{
public:
/**
 * @brief
 *
 * @fn BodyManager
 * @param manager
 */
    BodyManager(AgentManager *manager);
    /**
     * @brief
     *
     * @fn resetNewSegmentProperties
     */
    void resetNewSegmentProperties();
    /**
     * @brief
     *
     * @fn setNewSegmentColor
     * @param color
     */
    void setNewSegmentColor(qreal color) { m_newSegment.setColor(color); }
    /**
     * @brief
     *
     * @fn setNewSegmentColorInherited
     * @param inherited
     */
    void setNewSegmentColorInherited(bool inherited) { m_newSegment.setColorInherited(inherited); }
    /**
     * @brief
     *
     * @fn setNewSegmentId
     * @param id
     */
    void setNewSegmentId(quint32 id) {m_newSegment.setId(id);}
    /**
     * @brief
     *
     * @fn setNewSegmentParentId
     * @param parent
     */
    void setNewSegmentParentId(quint32 parent) {m_newSegment.setParentId(parent);}
    /**
     * @brief
     *
     * @fn setNewSegmentTranslation
     * @param translation
     */
    void setNewSegmentTranslation(QVector3D &translation) {m_newSegment.setTranslation(translation);}
    /**
     * @brief
     *
     * @fn setNewSegmentRestTranslation
     * @param restTranslation
     */
    void setNewSegmentRestTranslation(QVector3D &restTranslation) {m_newSegment.setRestTranslation(restTranslation);}
    /**
     * @brief
     *
     * @fn setNewSegmentRotation
     * @param rotation
     */
    void setNewSegmentRotation(QVector3D &rotation) {m_newSegment.setRotation(rotation);}
    /**
     * @brief
     *
     * @fn setNewSegmentRestRotation
     * @param restRotation
     */
    void setNewSegmentRestRotation(QVector3D &restRotation) {m_newSegment.setRestRotation(restRotation);}
    /**
     * @brief
     *
     * @fn setNewSegmentScale
     * @param scale
     */
    void setNewSegmentScale(QVector3D &scale) {m_newSegment.setScale(scale);}
    /**
     * @brief
     *
     * @fn setNewSegmentType
     * @param type
     */
    void setNewSegmentType(BrainiacGlobals::SegmentType type) {m_newSegment.setType(type);}
    /**
     * @brief
     *
     * @fn setNewSegmentName
     * @param name
     */
    void setNewSegmentName(QString &name) {m_newSegment.setName(name);}
    /**
     * @brief
     *
     * @fn setNewSegmentRotationTranslationOrder
     * @param order
     */
    void setNewSegmentRotationTranslationOrder(QList<BrainiacGlobals::RotTrans> order );

    /**
     * @brief creates a new segment
     *
     * Before the new segment will be created, it will be checked if
     *   - if this is a root node and there is already another root node nothing is added and false will be returned.
     *   - if no id has been set, a unique id is created and registered
     *   - if the id is already set nothing is added and false will be returned
     *
     * @sa IdGenerator
     * @sa AgentManager
     *
     * @fn createNewSegment
     * @return bool
     */
    bool createNewSegment();
    /**
     * @brief
     * @fn ~BodyManager
     */
    ~BodyManager();
protected:

    /**
     * @brief Adds a SegmentShape to all Agents of this AgentManager
     *
     * @fn addSegmentToAgents
     * @param id
     */
    void addSegmentToAgents(quint32 id);
    AgentManager *m_agentManager; /**< The AgentManager this BodyManagers belongs to */
    Segment m_newSegment; /**< Temporary Segment */
    QHash<quint32, SegmentShape *> m_segments; /**< TODO */

};

#endif // BODYMANAGER_H
