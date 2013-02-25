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
class Agent;

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
     * @brief applyBodyOnAgent
     * Apply the managed body to agent
     * @param agent the Agent this body should be applied
     */
    void applyBodyOnAgent(Agent *agent);

    /**
     * @brief getSegment
     * returns the Segment
     *
     * If there is no Segment with given id an empty Segment with id 0 is returned
     * @param id the id of the segment
     * @return the Segment
     */
    Segment getSegment(quint32 id) const;

    /**
     * @brief getSegmentChildrenIds
     * Returns the ids of all children of a Segment
     * @param id the id of the Segment
     * @return QList<quint32> an array of all ids
     */
    QList<quint32> getSegmentChildrenIds(quint32 id) const;

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

    Segment getRootSegment() const;

    QVector3D getSegmentRestRotation(quint32 id) const;

    QVector3D getSegmentRotation(quint32 id) const;

    QVector3D getSegmentScale(quint32 id) const;

    QVector3D getSegmentRestTranslation(quint32 id) const;

    QVector3D getSegmentTranslation(quint32 id) const;

    void setSegmentScale(quint32 id, qreal x, qreal y, qreal z);

    void setSegmentRestRotation(quint32 id, qreal x, qreal y, qreal z);

    void setSegmentRotation(quint32, qreal y, qreal y, qreal z);

    void setSegmentRestTranslation(quint32 id, qreal x, qreal y, qreal z);

    void setSegmentTranslation(quint32, qreal y, qreal y, qreal z);


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

    /**
     * @brief addSegmentsToAgent
     * Adds all segments to an Agent
     * @param agent the Agent
     * @param startSegmentId the id of the segment to start the recursion
     */
    void addSegmentsToAgent(Agent *agent, quint32 startSegmentId);

    AgentManager *m_agentManager; /**< The AgentManager this BodyManagers belongs to */
    Segment m_newSegment; /**< Temporary Segment */
    QHash<quint32, SegmentShape *> m_segments; /**< TODO */

};

#endif // BODYMANAGER_H
