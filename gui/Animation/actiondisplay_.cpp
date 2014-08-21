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

#include "actiondisplay_.h"

#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/body.h"



ActionDisplay_::ActionDisplay_():
    OsgMultithreadedViewerWidget(0,0,false)
{
    //setParent(parent);
    setWindowFlags(Qt::Tool);
    setWindowTitle("Action Editor");
    hide();
    setFocusPolicy(Qt::StrongFocus);
    //getGlWindow()->getGLWidget()->installEventFilter(new KeyPressEater());
    connect(getGlWindow()->getKeyMouseEater(),SIGNAL(keyPressed(Qt::Key)),this,SLOT(keyPressed(Qt::Key)));
    m_followAgent=false;

}

void ActionDisplay_::followUnfollowAgent()
{
    if(m_followAgent) {
        osg::Matrixd camMatrix;
        camMatrix.set(getCameraManipulator()->getMatrix());
        m_followDist.set(m_agentManager->getActionAgent()->getPosition().x()-camMatrix.getTrans().x(),m_agentManager->getActionAgent()->getPosition().y()-camMatrix.getTrans().y(),m_agentManager->getActionAgent()->getPosition().z()-camMatrix.getTrans().z());
        qDebug() << __PRETTY_FUNCTION__ << "Track on" << m_followDist.x() << m_followDist.y() << m_followDist.z();
    } else {
        qDebug() << __PRETTY_FUNCTION__ << "Track off";
    }
}

void ActionDisplay_::keyPressed(Qt::Key key)
{
    if(key==BrainiacGlobals::AnimationPauseKey) {
        emit animationRunningToggled();
    } else if(key==BrainiacGlobals::AnimationOneFrameForwardKey) {
        emit animationOneFrameBackward();
    } else if(key==BrainiacGlobals::AnimationOneFrameBackwardKey) {
        emit animationOneFrameForward();
    } else if(key==BrainiacGlobals::ToogleFollowAgentKey) {
        m_followAgent=!m_followAgent;
        followUnfollowAgent();
    } else if(key==BrainiacGlobals::ToggleAgentPositionMarkerKey) {
        m_agentManager->getActionAgent()->getBody()->toggleShowPositionMarker();
    } else if(key==BrainiacGlobals::ToggleSegmentCoordCrossesKey) {
        m_agentManager->getActionAgent()->getBody()->toggleShowCoordCrosses();
    }
}

//void ActionDisplay_::keyPressEvent(QKeyEvent *e)
//{
//    //OsgMultithreadedViewerWidget::keyPressEvent(e);
//    qDebug() << "Böa";
//    if(e->key()==Qt::Key_F) {
//        m_followAgent=!m_followAgent;
//    } else if(e->key()==Qt::Key_Space) {
//        emit animationRunningToggled();
//    } else if(e->key()==Qt::Key_Left) {
//        emit animationOneFrameBackward();
//    } else if(e->key()==Qt::Key_Right) {
//        emit animationOneFrameForward();
//    } else if(e->key()==Qt::Key_R) {
//        m_agentManager->getActionAgent()->setTranslation(0.0f,0.0f,0.0f);
//    }
//}


void ActionDisplay_::setAgentManager(AgentManager *agentManager)
{
    m_followAgent=false;
    m_agentManager=agentManager;
    m_sceneNode->removeChildren(0,m_sceneNode->getNumChildren());
    m_sceneNode->addChild(m_agentManager->getActionAgent()->getBody()->getBodyRoot());
    followUnfollowAgent();
}

void ActionDisplay_::updateCameraMatrix()
{
    if(m_followAgent) {
        osg::Matrixd camMatrix;
        camMatrix.set(getCameraManipulator()->getMatrix());
        camMatrix.setTrans(m_agentManager->getActionAgent()->getPosition().x()-m_followDist.x(),m_agentManager->getActionAgent()->getPosition().y()-m_followDist.y(),m_agentManager->getActionAgent()->getPosition().z()-m_followDist.z());
        getCameraManipulator()->setByMatrix(camMatrix);
    }
}
