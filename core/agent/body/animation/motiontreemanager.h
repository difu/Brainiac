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

#ifndef MOTIONTREEMANAGER_H
#define MOTIONTREEMANAGER_H

#include <QObject>
#include <QHash>
#include "core/idgenerator.h"


class AgentManager;
class MotionTree;
class QXmlStreamWriter;
class QXmlStreamReader;

class MotionTreeManager : public QObject
{
    Q_OBJECT
public:
    explicit MotionTreeManager(AgentManager *agentManager, QObject *parent = 0);
    void addDefaultMotionVariable( QString variable );

    /**
     * @brief adds a trigger
     *
     * an output channel for every agent with the name of this trigger is added (if not exists)
     *
     * @param QString triggerName the name of this trigger
     * @param quint32 id the id. If no id is given an unique id will be generated
     * @fn getAbsoluteFileDir
     * @return quint32 the unique id
     */
    quint32 addTrigger(QString triggerName, quint32 id=0);
    void deleteDefaultMotionVariable( QString variable );
    AgentManager* getAgentManager() const { return m_agentManager; }
    QList<QString> getDefaultMotionVariableNames() const { return m_defaultMotionVariableNames; }
    QHash<quint32,MotionTree *> getMotionTrees() const { return m_motionTrees; }
    QHash<quint32,QString> getTriggers() const { return m_triggers; }
    void loadConfig(QXmlStreamReader *stream);
    void saveConfig(QXmlStreamWriter *stream);
    static quint32 NUM_OF_TREE_TRACKS;
    virtual ~MotionTreeManager();
protected:
    void createTrees();
    AgentManager *m_agentManager; /**< the AgentManager this MotionTreeManager belongs to */
    QHash<quint32,MotionTree *> m_motionTrees; /**< Hash of all MotionTrees */
    QHash<quint32, QString> m_triggers; /**< Hash of tree triggers */
    QList<QString> m_defaultMotionVariableNames; /**< default motion variables names of all trees */
    IdGenerator m_latchIdGenerator;
signals:

public slots:

};

#endif // MOTIONTREEMANAGER_H
