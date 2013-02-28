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

ActionDisplay_::ActionDisplay_(QWidget *parent):
    OsgMultithreadedViewerWidget(0)
{
    setParent(parent);
    setWindowFlags(Qt::Tool);
    setWindowTitle("Action Editor");
    hide();
    setFocusPolicy(Qt::StrongFocus);
    //getGlWindow()->getGLWidget()->installEventFilter(new KeyPressEater());
    connect(getGlWindow()->getKeyPressedReleasedEater(),SIGNAL(keyPressed(Qt::Key)),this,SLOT(keyPressed(Qt::Key)));

}

void ActionDisplay_::keyPressed(Qt::Key key)
{
    if(key==Qt::Key_Space) {
        emit animationRunningToggled();
    } else if(key==Qt::Key_Left) {
        emit animationOneFrameBackward();
    } else if(key==Qt::Key_Right) {
        emit animationOneFrameForward();
    } else if(key==Qt::Key_F) {
        m_followAgent=!m_followAgent;
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
    m_agentManager=agentManager;
    m_rootNode->removeChildren(0,m_rootNode->getNumChildren());
    m_rootNode->addChild(m_agentManager->getActionAgent()->getBody()->getRootSegment());
}
