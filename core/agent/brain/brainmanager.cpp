// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2014  Dirk Fuchs dirkfux@googlemail.com

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


#include "brainmanager.h"
#include "core/agent/agentmanager.h"
#include "core/brainiaclogger.h"

BrainManager::BrainManager(QObject *parent) :
    QObject(parent)
{
}

AgentManager* BrainManager::getAgentManager() const
{
    return qobject_cast<AgentManager *>(parent());
}

const QPoint& BrainManager::getEditorFuzzyLocation(BrainiacGlobals::BrainiacId id) const
{
    Q_ASSERT(m_fuzzyProperties.contains(id));
    return m_fuzzyProperties.value(id)->getEditorLocation();
}

const QString& BrainManager::getFuzzyChannelName(BrainiacGlobals::BrainiacId id) const
{
    Q_ASSERT(m_fuzzyProperties.contains(id));
    return m_fuzzyProperties.value(id)->getChannelName();

}

const QString& BrainManager::getFuzzyName(BrainiacGlobals::BrainiacId id) const
{
    Q_ASSERT(m_fuzzyProperties.contains(id));
    return m_fuzzyProperties.value(id)->getName();
}

BrainiacGlobals::BrainiacId BrainManager::getFuzzyId(const QString &name) const
{
    QHash<BrainiacGlobals::BrainiacId, FuzzyProperties *>::const_iterator i;
    for (i = m_fuzzyProperties.constBegin(); i != m_fuzzyProperties.constEnd(); ++i) {
        FuzzyProperties *fp=i.value();
        if(fp->getName()==name) {
            return i.key();
        }
    }
    qCDebug(bAgent) << __PRETTY_FUNCTION__ << "Fuzz with name " << name << "not found";
    return 0;
}

void BrainManager::insertNewFuzzy(BrainiacGlobals::BrainiacId id)
{
    FuzzyProperties *props=new FuzzyProperties;
    m_fuzzyProperties.insert(id,props);
}

void BrainManager::setFuzzyEditorTranslation(BrainiacGlobals::BrainiacId id, qint32 x, qint32 y)
{
    if(!m_fuzzyProperties.contains(id)) {
        insertNewFuzzy(id);
    }
    FuzzyProperties *props=m_fuzzyProperties.value(id);
    props->setEditorLocation(QPoint(x,y));
}

void BrainManager::setFuzzyChannelName(BrainiacGlobals::BrainiacId id, const QString &channelName)
{
    if(!m_fuzzyProperties.contains(id)) {
        insertNewFuzzy(id);
    }
    FuzzyProperties *props=m_fuzzyProperties.value(id);
    props->setChannelName(channelName);
}

void BrainManager::setFuzzyName(BrainiacGlobals::BrainiacId id, const QString &name)
{
    if(!m_fuzzyProperties.contains(id)) {
        insertNewFuzzy(id);
    }
    FuzzyProperties *props=m_fuzzyProperties.value(id);
    props->setName(name);
}
