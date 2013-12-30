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

#include "motiontree.h"
#include "core/agent/body/animation/motiontreemanager.h"
#include "core/agent/body/animation/motiontreeaction.h"
#include "core/agent/body/animation/motiontreetransition.h"
#include "core/agent/agentmanager.h"
#include "gui/Animation/motiontreeeditor.h"


MotionTree::MotionTree(QObject *parent) :
    QObject(parent)
{
    m_motionTreeManager=dynamic_cast<MotionTreeManager *>(parent);
    if(!m_motionTreeManager) {
        qFatal("Failed to cast *QOBJECT to *MotionTreeManager in MotionTree::MotionTree(QObject *parent)");
    }
    m_motionTreeEditor=new MotionTreeEditor(this, m_motionTreeManager->getAgentManager()->getScene());
}

QString MotionTree::addAction(QString name)
{
    while(true) {
        if( m_actions.contains(name)) {
            name.append("_");
        } else {
            break;
        }
    }
    MotionTreeAction *action=new MotionTreeAction(this);
    m_actions.insert(name,action);
    return name;
}


QString MotionTree::addTransition(QString name) {
    while(true) {
        if( m_transitions.contains(name)) {
            name.append("_");
        } else {
            break;
        }
    }
    MotionTreeTransition *trans=new MotionTreeTransition(this);
    m_transitions.insert(name,trans);
    return name;
}

bool MotionTree::connectActionWithTransition(QString action, QString transition)
{
    // Check, if action/transition pair already exists
    MotionTreeAction *act=m_actions.value(action,0);
    if(!act) {
        return false;
    }
    MotionTreeTransition *trans=m_transitions.value(transition,0);
    if(!trans) {
        return false;
    }

    bool found=false;
    QHash<MotionTreeAction *, MotionTreeTransition *>::const_iterator i=m_actionTransitionConnections.find(act);
    while( i!=m_actionTransitionConnections.end() && i.value()==trans) {
        found=true;
        ++i;
    }
    if(found) {
        return false;
    }

    if(m_actions.contains(action) && m_transitions.contains(transition)) {
        m_actionTransitionConnections.insertMulti(act,trans);
        return true;
    }
    return false;
}

bool MotionTree::connectTransitionWithAction(QString transition, QString action)
{
    // Check, if action/transition pair already exists
    MotionTreeAction *act=m_actions.value(action,0);
    if(!act) {
        return false;
    }
    MotionTreeTransition *trans=m_transitions.value(transition,0);
    if(!trans) {
        return false;
    }

    bool found=false;
    QHash<MotionTreeTransition *,MotionTreeAction *>::const_iterator i=m_transitionActionConnections.find(trans);
    while( i!=m_transitionActionConnections.end() && i.value()==act) {
        found=true;
        ++i;
    }
    if(found) {
        return false;
    }

    if(m_actions.contains(action) && m_transitions.contains(transition)) {
        m_transitionActionConnections.insertMulti(trans,act);
        return true;
    }
    return false;
}


MotionTree::~MotionTree()
{
    foreach(MotionTreeAction *act, m_actions) {
        delete act;
    }
    foreach(MotionTreeTransition *trans, m_transitions) {
        delete trans;
    }
}
