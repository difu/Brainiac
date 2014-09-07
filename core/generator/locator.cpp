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


#include "locator.h"
#include "core/generator/generator.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/group/group.h"

Locator::Locator(Group *group, Generator *generator, qreal x, qreal y, qreal z, qreal w):
    QObject(0),
    m_position(QVector4D(x,y,z,w)),
    m_generator(generator),
    m_group(group)
{
}

Agent* Locator::createInstance()
{
    Agent *agent=getAgent();
    if(agent) {
        return agent;
    } else {
        if(m_group->getAgentManager()) {
            agent=m_group->createAndAddNewAgent();
            setAgent(agent);
            agent->reset();
            QString name;
            name=m_group->getName()+QString::number(agent->getId());
            agent->setObjectName(name);
            return agent;
        }
    }
    qWarning() << __PRETTY_FUNCTION__ << "no AgentManager found";
    return 0;
}

Agent* Locator::getAgent()
{
    if(hasInstance()) {
        return qobject_cast<Agent *>(children().first());
    }
    return 0;
}

bool Locator::hasInstance() const
{
    int numChildren=children().count();
    Q_ASSERT(numChildren<=1);
    if(numChildren!=1) {
        return false;
    }
    if(qobject_cast<Agent *>(children().first())) {
        return true;
    }
    return false;
}

QVector4D Locator::getLocation() const
{
    return QVector4D(m_position.x()+m_generator->getCenter().x(),
                     m_position.y()+m_generator->getCenter().y(),
                     m_position.z()+m_generator->getCenter().z(),
                     m_position.w());
}

QVector4D& Locator::getLocationRelativeToCenter()
{
    return m_position;
}

void Locator::setAgent(Agent *agent)
{
    agent->setParent(this);
}

Locator::~Locator()
{
    //delete m_position;
}
