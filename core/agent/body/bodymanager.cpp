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

#include "bodymanager.h"
#include "core/agent/agentmanager.h"


BodyManager::BodyManager(AgentManager *manager)
{
    m_agentManager=manager;
    resetNewSegmentProperties();
}

bool BodyManager::createNewSegment()
{
    // first check, if everything is valid

    // If this should be a root node, check if we have already a root node
    if(m_newSegment.isRootElement()) {
        foreach(Segment *seg, m_segments) {
            if(seg->isRootElement()) {
                qDebug() << __PRETTY_FUNCTION__ << "Error: Trying to add a second root segment";
                resetNewSegmentProperties();
                return false;
            }
        }
    }
    // check, if the new segments id is already in use
    if(m_segments.contains(m_newSegment.getId())) {
        qDebug() << __PRETTY_FUNCTION__ << "Error: Id is already in use";
        resetNewSegmentProperties();
        return false;
    }
    if(m_newSegment.getId()==0) {
        m_newSegment.setId(m_agentManager->getSegmentIdGenerator().getNewId());
    } else {
        m_agentManager->getSegmentIdGenerator().registerId(m_newSegment.getId());
    }

    SegmentShape *newSegment=new SegmentShape(m_newSegment);
    m_segments.insert(newSegment->getId(),newSegment);
    resetNewSegmentProperties();
    return true;
}

void BodyManager::resetNewSegmentProperties()
{
    m_newSegment.setColor(1.0f);
    m_newSegment.setColorInherited(false);
    m_newSegment.setRestRotation(QVector3D());
    m_newSegment.setRotation(QVector3D());
    m_newSegment.setTranslation(QVector3D());
    m_newSegment.setRestTranslation(QVector3D());
    m_newSegment.setScale(QVector3D());
    m_newSegment.setName("New Segment");
    m_newSegment.setId(0);
    m_newSegment.setParentId(0);
}

void BodyManager::setNewSegmentRotationTranslationOrder(QList<BrainiacGlobals::RotTrans> order)
{
    m_newSegment.setRotationTranslationOrder(order);
}

 BodyManager::~BodyManager()
{
    foreach(Segment *s,m_segments) {
        s->deleteLater();
    }
    m_segments.clear();
}
