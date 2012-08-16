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


#include "brainiacdisplay.h"
#include <QMouseEvent>
#include "core/brainiacglobals.h"

BrainiacDisplay::BrainiacDisplay(QWidget *parent) :
    QGLView(parent)
{
    m_camera=new QGLCamera();
    setWindowFlags(Qt::Tool);
    setGeometry(this->geometry().x(),this->geometry().y(),600,400);
    m_camera->setFarPlane(1000000);
    setCamera(m_camera);
    m_camera->setCenter(QVector3D(0,100,0));
    m_camera->setEye(QVector3D(-500,100,0));
    setOption(QGLView::CameraNavigation,false);
    m_shiftPressed=false;
    m_renderSkeleton=false;
    m_showCoordCross=false;
    m_showAgentBoneCoordCrosses=false;
}

void BrainiacDisplay::keyPressEvent(QKeyEvent *event)
{
    QGLView::keyPressEvent(event);
    if(event->key()==Qt::Key_Shift) {
        m_shiftPressed=true;
    } else if(event->key()==Qt::Key_C) {
        if(m_shiftPressed) {
            m_showCoordCross=!m_showCoordCross;
        } else {
            m_showAgentBoneCoordCrosses=!m_showAgentBoneCoordCrosses;
        }
    } else if(event->key()==Qt::Key_S) {
        if(!m_shiftPressed) {
            m_renderSilhouettes=!m_renderSilhouettes;
        } else {
            m_renderSkeleton=!m_renderSkeleton;
        }
    }
}

void BrainiacDisplay::keyReleaseEvent(QKeyEvent *event) {
    QGLView::keyReleaseEvent(event);
    if(event->key()==Qt::Key_Shift) {
        m_shiftPressed=false;
    }
}

void BrainiacDisplay::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();
    if (event->buttons() & Qt::MidButton) {
        m_camera->rotateCenter(m_camera->pan((qreal)dx/2.0f));
    } else if (event->buttons() & Qt::RightButton)  {
        if(m_shiftPressed) {
            QVector3D v=m_camera->eye()-m_camera->center();
            v*=.02f*-dx;
            v=m_camera->translation(v.x(),v.y(),v.z());
            m_camera->translateEye(v.x(),v.y(),v.z());
            m_camera->translateCenter(v.x(),v.y(),v.z());
            //qDebug() << __PRETTY_FUNCTION__ << m_camera->eye() << m_camera->center();
        } else {
            //m_camera->rotateEye(m_camera->pan((qreal)dx/2.0f) * m_camera->tilt( (qreal)dy/2.0f));
            m_camera->rotateEye(m_camera->tilt( (qreal)dy/2.0f));
            m_camera->rotateEye(m_camera->pan((qreal)dx/2.0f));
            //m_camera->setUpVector(QVector3D(0,1,0));
        }
        update();
    }
    m_lastPos = event->pos();
}

void BrainiacDisplay::mousePressEvent(QMouseEvent *event)
{
    QGLView::mouseMoveEvent(event);
    m_lastPos = event->pos();
    this->setFocus();
}

void BrainiacDisplay::paintGL(QGLPainter *painter)
{
    QVector3DArray vertices;
    static qreal size=1000.0f;
    static qreal width=10.0f;
    vertices.append(size,0,size);
    vertices.append(size,0,-size);
    vertices.append(-size,0,-size);
    vertices.append(-size,0,size);

    int count=0;
    for(qreal i=0.0f;i<size*2.f;i+=width) {
        vertices.append(-size+i,0,size);
        vertices.append(-size+i,0,-size);
        vertices.append(size,0,-size+i);
        vertices.append(-size,0,-size+i);
        count++;
    }

    painter->clearAttributes();
    painter->setStandardEffect(QGL::FlatColor);
    painter->setVertexAttribute(QGL::Position, vertices);
    painter->setColor(QColor(30,30,30));
    painter->draw(QGL::LineLoop,4,0);
    painter->draw(QGL::Lines,count*4,4);

    painter->clearAttributes();
    vertices.clear();

    static qreal crossLength=150.0f;

    if(m_showCoordCross) {
        vertices.append(0,1,0);
        vertices.append(crossLength,1,0);
        vertices.append(0,0,0);
        vertices.append(0,crossLength,0);
        vertices.append(0,1,0);
        vertices.append(0,1,crossLength);

        painter->setStandardEffect(QGL::FlatColor);
        painter->setVertexAttribute(QGL::Position, vertices);

        painter->setColor(BrainiacGlobals::defaultXColor);
        painter->draw(QGL::Lines,2,0);
        painter->setColor(BrainiacGlobals::defaultYColor);
        painter->draw(QGL::Lines,2,2);
        painter->setColor(BrainiacGlobals::defaultZColor);
        painter->draw(QGL::Lines,2,4);
    }
}
