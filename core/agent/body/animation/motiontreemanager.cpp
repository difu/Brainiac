// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2013  Dirk Fuchs dirkfux@googlemail.com

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

#include "motiontreemanager.h"

#include "core/agent/body/animation/motiontree.h"
#include "core/agent/agentmanager.h"

MotionTreeManager::MotionTreeManager(AgentManager *agentManager, QObject *parent) :
    QObject(parent),m_agentManager(agentManager)
{
    createChannels();
}

void MotionTreeManager::createChannels()
{
    for(quint32 i=0;i<NUM_OF_TREE_TRACKS;i++) {
        MotionTree *mt=new MotionTree(this);
        m_motionTrees.insert(i,mt);
    }
}

quint32 MotionTreeManager::NUM_OF_TREE_TRACKS=8;

MotionTreeManager::~MotionTreeManager()
{
    foreach(MotionTree *mt, m_motionTrees) {
        mt->deleteLater();
    }
}
