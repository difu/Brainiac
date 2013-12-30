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

#ifndef MOTIONTREE_H
#define MOTIONTREE_H

#include <QObject>
#include <QHash>

class AgentManager;
class MotionTreeManager;
class MotionTreeEditor;
class MotionTreeAction;
class MotionTreeTransition;

class MotionTree : public QObject
{
    Q_OBJECT
public:
    explicit MotionTree(QObject *parent = 0);
    virtual ~MotionTree();
    QHash<QString, MotionTreeAction *> getActions() const { return m_actions; }
    QHash<QString, MotionTreeTransition *> getTransitions() const { return m_transitions; }
    MotionTreeEditor* getMotionTreeEditor() const { return m_motionTreeEditor; }
    QString addAction(QString name="Action");
    QString addTransition(QString name="Transition");
    bool connectActionWithTransition( QString action, QString transition);
    bool connectTransitionWithAction( QString transition, QString action);

protected:
    AgentManager *m_agentManager;
    MotionTreeManager *m_motionTreeManager;
    MotionTreeEditor *m_motionTreeEditor;
    QHash<QString, MotionTreeAction *> m_actions;
    QHash<QString, MotionTreeTransition *> m_transitions;
    QHash<MotionTreeAction *, MotionTreeTransition *> m_actionTransitionConnections;
    QHash<MotionTreeTransition *, MotionTreeAction *> m_transitionActionConnections;
signals:

public slots:

};

#endif // MOTIONTREE_H
