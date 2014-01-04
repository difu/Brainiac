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
    void deleteDefaultMotionVariable( QString variable );
    AgentManager* getAgentManager() const { return m_agentManager; }
    QList<QString> getDefaultMotionVariableNames() const { return m_defaultMotionVariableNames; }
    QHash<quint32,MotionTree *> getMotionTrees() const { return m_motionTrees; }
    void loadConfig(QXmlStreamReader *stream);
    void saveConfig(QXmlStreamWriter *stream);
    static quint32 NUM_OF_TREE_TRACKS;
    virtual ~MotionTreeManager();
protected:
    void createTrees();
    AgentManager *m_agentManager; /**< the AgentManager this MotionTreeManager belongs to */
    QHash<quint32,MotionTree *> m_motionTrees; /**< Hash of all MotionTrees */
    QList<QString> m_defaultMotionVariableNames; /**< default motion variables names of all trees */
signals:

public slots:

};

#endif // MOTIONTREEMANAGER_H
