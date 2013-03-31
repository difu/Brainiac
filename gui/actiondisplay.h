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


#ifndef ACTIONDISPLAY_H
#define ACTIONDISPLAY_H
#include "gui/brainiacdisplay.h"
#include <QCloseEvent>

class Agent;
class Camera;
class Scene;
class QGLAbstractScene;
class QGLCamera;
class AgentManager;

/**
 * @brief The OpenGL view of the agent which agents are edited by the ActionEditor
 *
 * @sa ActionEditor
 * @class ActionDisplay actiondisplay.h "gui/actiondisplay.h"
 */
class ActionDisplay  : public BrainiacDisplay
{
    Q_OBJECT
public:
    ActionDisplay(QWidget *parent);
    /**
     * @brief sets the Agent to display
     *
     * @fn setAgent
     * @param agent the agent
     */
    void setAgent(Agent *agent);
    void setCameraOffset(const QVector3D &offset);

protected:
    void keyPressEvent(QKeyEvent *e);
    void paintGL(QGLPainter *painter);
    QVector3D m_cameraCenterPos;
    QVector3D m_cameraEyePos;

    Agent *m_agent;
    bool m_followAgent;
signals:
    void animationRunningToggled();
    void animationOneFrameForward();
    void animationOneFrameBackward();
};

#endif // ACTIONDISPLAY_H
