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


#include "bodydisplay.h"
#include "core/scene.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"

#include <QKeyEvent>

BodyDisplay::BodyDisplay(Scene *scene): BrainiacDisplay(0)
{
    m_scene=scene;
    m_rootSkeletonNode=0;
    m_agentManager=0;
    m_renderSilhouettes=false;
    m_renderSkeleton=false;
//    setWindowFlags(Qt::Tool);
//    setGeometry(this->geometry().x(),this->geometry().y(),600,400);
   show();
   setWindowTitle("Body");
}

void BodyDisplay::setAgentManager(AgentManager *manager)
{
    m_agentManager=manager;
    if(m_agentManager) {
        m_rootSkeletonNode=m_agentManager->getMasterAgent()->getBody()->getRootSkeletonNode();
        qDebug() << m_agentManager->getName() << "in BodyDisplay";
    }
    setWindowTitle("Body: "%m_agentManager->getName());
}

void BodyDisplay::keyPressEvent(QKeyEvent *e)
{
    BrainiacDisplay::keyPressEvent(e);
    if(m_agentManager) {
        m_agentManager->getMasterAgent()->getBody()->showSilhouettes(m_renderSilhouettes);
        m_agentManager->getMasterAgent()->getBody()->showBoneCoordCrosses(m_showAgentBoneCoordCrosses);
        updateGL();
    }
//    if(e->key()==Qt::Key_S) {
//        if(!m_shiftPressed) {
//            if(m_agentManager) {
//                m_renderSilhouettes=!m_renderSilhouettes;
//                m_agentManager->getMasterAgent()->getBody()->showSilhouettes(m_renderSilhouettes);
//                updateGL();
//            }
//        } else {
//            m_renderSkeleton=!m_renderSkeleton;
//            updateGL();
//        }
//    }
}

void BodyDisplay::paintGL(QGLPainter *painter)
{
    BrainiacDisplay::paintGL(painter);
    painter->setStandardEffect(QGL::LitMaterial);
    //painter->setFaceColor(QGL::AllFaces, QColor(170, 202, 0));
    //m_rootSceneNode->draw(painter);
    if(m_rootSkeletonNode) {
        m_rootSkeletonNode->draw(painter);
        if(m_renderSkeleton) {
            m_agentManager->getMasterAgent()->getBody()->renderSkeleton(painter);
        }
    }
}

