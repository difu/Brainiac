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


#ifndef BRAINEDITOR_H
#define BRAINEDITOR_H

#include <QGraphicsScene>
#include "gui/editorbase.h"

class AgentManager;
class Agent;
class BrainEditorItem;

class BrainEditor : public EditorBase
{
    Q_OBJECT
public:
    explicit BrainEditor(Scene *scene, AgentManager *agentManager);
    Agent* getSelectedAgent();
    void setSelectedAgent(Agent *agent);
    void updateItemLocations();

protected:
    void deleteSelectedItems();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    bool m_altPressed; //!< true, if alt key is pressed
    bool m_shiftPressed; //!< true, if shift key is pressed
    BrainEditorItem *m_connectSourceItem; //!< holds the source fuzz when in connection mode
    Agent *m_selectedAgent; //!< the currently selected agent of this editor. This agent´s fuzz results are displayed
    AgentManager *m_agentManager; //!< the agentmanager of this braineditor
signals:
    void statusBarMessageChanged( const QString & message );
    void itemsDelete();
public slots:

};

#endif // BRAINEDITOR_H
