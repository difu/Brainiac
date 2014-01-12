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

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "core/agent/body/animation/motiontree.h"
#include "core/agent/body/animation/motiontreeaction.h"
#include "core/agent/body/animation/motiontreetransition.h"
#include "core/agent/agentmanager.h"

MotionTreeManager::MotionTreeManager(AgentManager *agentManager, QObject *parent) :
    QObject(parent),m_agentManager(agentManager)
{
    createTrees();
}

void MotionTreeManager::addDefaultMotionVariable(QString variable)
{
    if(!m_defaultMotionVariableNames.contains(variable))
        m_defaultMotionVariableNames.append(variable);
}

void MotionTreeManager::deleteDefaultMotionVariable(QString variable)
{
    int index=m_defaultMotionVariableNames.indexOf(variable);
    if(index!=-1) {
        m_defaultMotionVariableNames.removeAt(index);
    }
}

void MotionTreeManager::createTrees()
{
    for(quint32 i=0;i<NUM_OF_TREE_TRACKS;i++) {
        MotionTree *mt=new MotionTree(this);
        m_motionTrees.insert(i,mt);
    }
}

void MotionTreeManager::loadConfig(QXmlStreamReader *stream)
{
    Q_ASSERT(stream->isStartElement() && stream->name() == BrainiacGlobals::XmlMotionTreesTag);
    while(stream->readNextStartElement()) {
        Q_ASSERT(stream->isStartElement() && stream->name() == BrainiacGlobals::XmlMotionTreeTag);
        QXmlStreamAttributes attribs = stream->attributes();
        int treeId=attribs.value(BrainiacGlobals::XmlIdAttrib).toInt();
        MotionTree *tree=getMotionTrees().value(treeId);
        tree->setTreeDefaultAction(attribs.value(BrainiacGlobals::XmlTreeDefaultActionAttrib).toString());
        while(stream->readNextStartElement()) {
            if(stream->name()==BrainiacGlobals::XmlTreeActionTag) {
                QXmlStreamAttributes attribs = stream->attributes();
                tree->addAction(attribs.value(BrainiacGlobals::XmlNameAttrib).toString());
            } else if(stream->name()==BrainiacGlobals::XmlTreeTransitionTag) {
                QXmlStreamAttributes attribs = stream->attributes();
                tree->addTransition(attribs.value(BrainiacGlobals::XmlNameAttrib).toString());
            } else if(stream->name()==BrainiacGlobals::XmlTreeActionTransitionConnectionTag) {
                QXmlStreamAttributes attribs = stream->attributes();
                tree->connectActionWithTransition(attribs.value(BrainiacGlobals::XmlTreeActionAttrib).toString(),attribs.value(BrainiacGlobals::XmlTreeTransitionAttrib).toString());
            } else if(stream->name()==BrainiacGlobals::XmlTreeTransitionActionConnectionTag) {
                QXmlStreamAttributes attribs = stream->attributes();
                tree->connectTransitionWithAction(attribs.value(BrainiacGlobals::XmlTreeTransitionAttrib).toString(),attribs.value(BrainiacGlobals::XmlTreeActionAttrib).toString());
            }
            stream->skipCurrentElement();
        }
        //stream->skipCurrentElement(); // XmlMotionTreeTag
    }

    //stream->skipCurrentElement(); // XmlMotionTreesTag
}

void MotionTreeManager::saveConfig(QXmlStreamWriter *stream)
{
    stream->writeStartElement(BrainiacGlobals::XmlMotionTreesTag); // MotionTrees
    for(unsigned int i=0; i<MotionTreeManager::NUM_OF_TREE_TRACKS; i++) {
        MotionTree *tree=getMotionTrees().value(i);
        stream->writeStartElement(BrainiacGlobals::XmlMotionTreeTag); // MotionTree
        stream->writeAttribute(BrainiacGlobals::XmlTreeDefaultActionAttrib,tree->getDefaultActionName());
        stream->writeAttribute(BrainiacGlobals::XmlIdAttrib,QString::number(i));

        // Actions
        foreach(MotionTreeAction *action, tree->getActions())  {
            stream->writeStartElement(BrainiacGlobals::XmlTreeActionTag);
            stream->writeAttribute(BrainiacGlobals::XmlNameAttrib,tree->getActionName(action));
            stream->writeEndElement(); // XmlTreeActionTag
        }
        // Transitions
        foreach(MotionTreeTransition *trans, tree->getTransitions())  {
            stream->writeStartElement(BrainiacGlobals::XmlTreeTransitionTag);
            stream->writeAttribute(BrainiacGlobals::XmlNameAttrib,tree->getTransitionName(trans));
            stream->writeEndElement(); // XmlTreeTransitionTag
        }

        QHashIterator<MotionTreeAction *, MotionTreeTransition *> itAT=tree->getActionTransitionConnections();
        while (itAT.hasNext()) {
            itAT.next();
            stream->writeStartElement(BrainiacGlobals::XmlTreeActionTransitionConnectionTag);
            stream->writeAttribute(BrainiacGlobals::XmlTreeActionAttrib,tree->getActionName(itAT.key()));
            stream->writeAttribute(BrainiacGlobals::XmlTreeTransitionAttrib,tree->getTransitionName(itAT.value()));
            stream->writeEndElement(); // ActionTransition
        }

        QHashIterator<MotionTreeTransition *, MotionTreeAction *> itTA=tree->getTransitionActionConnections();
        while (itTA.hasNext()) {
            itTA.next();
            stream->writeStartElement(BrainiacGlobals::XmlTreeTransitionActionConnectionTag);
            stream->writeAttribute(BrainiacGlobals::XmlTreeTransitionAttrib,tree->getTransitionName(itTA.key()));
            stream->writeAttribute(BrainiacGlobals::XmlTreeActionAttrib,tree->getActionName(itTA.value()));
            stream->writeEndElement(); // TransitionAction
        }

        stream->writeEndElement(); // MotionTree
    }

    stream->writeEndElement(); // MotionTrees
}

quint32 MotionTreeManager::NUM_OF_TREE_TRACKS=8;

MotionTreeManager::~MotionTreeManager()
{
    foreach(MotionTree *mt, m_motionTrees) {
        mt->deleteLater();
    }
}
