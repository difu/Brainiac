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
#include "core/agent/agent.h"
#include "core/agent/channel.h"
#include "core/agent/body/body.h"
#include "core/agent/body/animation/motiontreeanimationplayer.h"
#include "core/agent/body/animation/animation.h"
#include "gui/Animation/motiontreeeditor.h"
#include "gui/Animation/motiontreeeditoritemconnector.h"


MotionTree::MotionTree(quint32 track, QObject *parent) :
    m_track(track),
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
        MotionTreeEditorItemConnector *connector=new MotionTreeEditorItemConnector((EditorItem *)act->getEditorItem(),(EditorItem *)trans->getEditorItem());
        m_motionTreeEditor->addItem(connector);
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
        MotionTreeEditorItemConnector *connector=new MotionTreeEditorItemConnector((EditorItem *)trans->getEditorItem(),(EditorItem *)act->getEditorItem());
        m_motionTreeEditor->addItem(connector);
        return true;
    }
    return false;
}

QString MotionTree::getActionName( MotionTreeAction *action ) const
{
    return m_actions.key(action,QString());
}

QString MotionTree::getTransitionName(MotionTreeTransition *transition) const
{
    return m_transitions.key(transition,QString());
}

void MotionTree::updateEditor()
{
    Agent *selectedAgent=m_motionTreeManager->getAgentManager()->getSelectedAgent();
    MotionTreeAnimationPlayer *player=selectedAgent->getBody()->getMotionTreeAnimationPlayers().at(m_track);
    Animation *currentAnim=player->getCurrentAnimation();
    if(currentAnim)
    {
        qreal phase=Channel::getInputValue(selectedAgent,BrainiacGlobals::ChannelNames_Phases.at(m_track));
        MotionTreeAction *treeAction=m_actions.value(currentAnim->name(),0);
        if(treeAction) {
            treeAction->setPhase(phase);
        } else {
            qDebug() << __PRETTY_FUNCTION__ << "could not get treeAction!";
        }
    } else {
         //qDebug() << __PRETTY_FUNCTION__ << "Could not get current animation!";
    }

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
