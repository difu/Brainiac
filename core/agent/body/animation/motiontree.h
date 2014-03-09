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
    typedef QHash<MotionTreeAction *, MotionTreeTransition *> ActionTransitionConnectionType;
    typedef QHash<MotionTreeTransition *, MotionTreeAction *> TransitionActionConnectionType;
    explicit MotionTree(quint32 track, QObject *parent = 0);
    virtual ~MotionTree();


    /**
     * @brief adds an action to this MotionTree
     *
     * @param QString name the name of this action
     * @returns QString the name this action was finally given
     *
     * if the name is already in use "_" are appended until the name is unique
     */
    QString addAction(QString name="Action");

    /**
     * @brief adds a transition to this MotionTree
     *
     * @param QString name the name of this transition
     * @returns QString the name this transition was finally given
     *
     * if the name is already in use "_" are appended until the name is unique
     */
    QString addTransition(QString name="Transition");
    bool connectActionWithTransition( QString action, QString transition);
    bool connectTransitionWithAction( QString transition, QString action);
    QHash<QString, MotionTreeAction *> getActions() const { return m_actions; }
    QHash<QString, MotionTreeTransition *> getTransitions() const { return m_transitions; }
    MotionTreeEditor* getMotionTreeEditor() const { return m_motionTreeEditor; }
    QList<MotionTreeAction *> getNextActionsFromAction(MotionTreeAction *action) const;
    QString getActionName(MotionTreeAction *action) const;
    const QString getActionName(MotionTreeAction *action);
    ActionTransitionConnectionType getActionTransitionConnections() const { return m_actionTransitionConnections; }
    TransitionActionConnectionType getTransitionActionConnections() const { return m_transitionActionConnections; }

    MotionTreeAction *getDefaultAction() const { return m_actions.value(m_defaultActionName,0); }
    QString getDefaultActionName() const { return m_defaultActionName; }
    quint32 getTrack() const { return m_track; }
    QString getTransitionName(MotionTreeTransition *transition) const;
    void setTreeDefaultAction( const QString &name ) { m_defaultActionName=name; }

protected:
    AgentManager *m_agentManager;
    MotionTreeManager *m_motionTreeManager;
    MotionTreeEditor *m_motionTreeEditor;
    QHash<QString, MotionTreeAction *> m_actions;
    QHash<QString, MotionTreeTransition *> m_transitions;
    ActionTransitionConnectionType m_actionTransitionConnections;
    TransitionActionConnectionType m_transitionActionConnections;
    QString m_defaultActionName;
    quint32 m_track;
signals:

public slots:
    void updateEditor();

};

#endif // MOTIONTREE_H
