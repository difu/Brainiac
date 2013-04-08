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


#include "actiondisplay.h"

#include "core/scene.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"

ActionDisplay::ActionDisplay(QWidget *parent): BrainiacDisplay(parent)
{
    m_agent=0;
    m_followAgent=false;
    m_renderSilhouettes=false;
    m_renderSkeleton=false;
    hide();
    setWindowTitle("Action Editor");
}

void ActionDisplay::keyPressEvent(QKeyEvent *e)
{
    BrainiacDisplay::keyPressEvent(e);
    if(e->key()==Qt::Key_F) {
        m_followAgent=!m_followAgent;
    } else if(e->key()==Qt::Key_Space) {
        emit animationRunningToggled();
    } else if(e->key()==Qt::Key_Left) {
        emit animationOneFrameBackward();
    } else if(e->key()==Qt::Key_Right) {
        emit animationOneFrameForward();
    } else if(e->key()==Qt::Key_R) {
        if(m_agent) {
            m_agent->setTranslation(0.0f,0.0f,0.0f);
        }
    }
    if(m_agent) {
        //m_agent->getBody()->showBoneCoordCrosses(m_showAgentBoneCoordCrosses);
    }
}

void ActionDisplay::paintGL(QGLPainter *painter)
{
    BrainiacDisplay::paintGL(painter);
    painter->setStandardEffect(QGL::LitMaterial);

    if(m_agent)
    {
        if(m_renderSkeleton) {
            //m_agent->getBody()->renderSkeleton(painter);
        }
        //m_agent->getBody()->getRootSkeletonNode()->draw(painter);
    }
    if(!m_followAgent) {
        m_cameraCenterPos=m_camera->center();
        m_cameraEyePos=m_cameraEyePos;
    }
}

void ActionDisplay::setAgent(Agent *agent)
{
    m_agent=agent;
}

void ActionDisplay::setCameraOffset(const QVector3D &offset)
{
    if(m_followAgent) {
        QVector3D v=m_camera->eye()-m_camera->center();
        m_camera->setCenter(offset+m_cameraCenterPos);
        m_camera->setEye(m_camera->center()+v);
        m_camera->setUpVector(QVector3D(0,1,0));
    }
}

