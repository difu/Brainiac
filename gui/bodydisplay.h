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


#ifndef BODYDISPLAY_H
#define BODYDISPLAY_H

#include "gui/brainiacdisplay.h"
#include <QCloseEvent>

class Camera;
class Scene;
class QGLAbstractScene;
class SkeletonNode;
class QGLCamera;
class AgentManager;

class BodyDisplay : public BrainiacDisplay
{
    Q_OBJECT
public:
    BodyDisplay(Scene *scene);
    void setAgentManager( AgentManager *manager );

protected:
    void keyPressEvent(QKeyEvent *e);
    void paintGL(QGLPainter *painter);

    SkeletonNode *m_rootSkeletonNode;
    Scene *m_scene;
    AgentManager *m_agentManager;
};

#endif // BODYDISPLAY_H
