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


#include "camera.h"
#include "core/brainiacglobals.h"
#include<QMouseEvent>
#include <QtOpenGL>
#include<QDebug>

Camera::Camera(Scene *scene, qreal xPos, qreal yPos, qreal zPos, qreal xRot, qreal yRot, qreal zRot)
{
    m_scene=scene;
    m_position.setX(xPos);
    m_position.setY(yPos);
    m_position.setZ(zPos);
    m_rotation.setX(xRot);
    m_rotation.setX(xRot);
    m_rotation.setY(yRot);
    m_rotation.setZ(zRot);
}

/** \brief move camera forward

**/
void Camera::moveCameraForward(qreal offset)
{
    setTranslationOffset(offset*BrainiacGlobals::sinGrad(-m_rotation.y()),offset*BrainiacGlobals::sinGrad(m_rotation.x()),offset*BrainiacGlobals::cosGrad(-m_rotation.y()));
}


/** \brief move camera sidewise

**/
void Camera::moveCameraSidewise(qreal offset)
{
    setTranslationOffset(offset*BrainiacGlobals::cosGrad(-m_rotation.y()),offset*BrainiacGlobals::sinGrad(-m_rotation.z()),offset*BrainiacGlobals::sinGrad(-m_rotation.y()));
}

/** \brief places camera in OpenGL context

**/
void Camera::placeCameraGL() {
    glRotatef(m_rotation.x(),1,0,0);
    glRotatef(m_rotation.y(),0,1,0);


    glTranslatef(-m_position.x(), -m_position.y(), -m_position.z());
}


/** \brief set camera´s rotation
                set the rotation of the camera, the angles are also normalized
                @param rot Rotation vector
                @param preventHeadOver if true x rotation will be limited to -90 <=x <= 90 to prevent head over

**/
void Camera::setRotation(QVector3D rot, bool preventHeadOver)
{
    setRotation(rot.x(),rot.y(),rot.z(),preventHeadOver);
}

/** \brief set camera´s rotation

                 set the rotation of the camera, the angles are also normalized
                 @param preventHeadOver if true x rotation will be limited to -90 <= x <= 90 to prevent head over
**/
void Camera::setRotation(qreal x, qreal y, qreal z, bool preventHeadOver)
{
    if(preventHeadOver) {
        x=qBound((qreal)-90.0f,x,(qreal)+90.0f); // Prevent "head over"
    } else {
        BrainiacGlobals::normalizeAngle(&x);
    }
    BrainiacGlobals::normalizeAngle(&y);
    BrainiacGlobals::normalizeAngle(&z);
    m_rotation.setX(x);
    m_rotation.setY(y);
    m_rotation.setZ(z);
}

/** \brief set camera´s rotation offset

                   the values are added to the current rotation and the angles are also normalized
                   @param preventHeadOver if true x rotation will be limited to -90 <= x <= 90 to prevent head over
**/
void Camera::setRotationOffset(qreal x, qreal y, qreal z, bool preventHeadOver)
{
    qreal rotx=m_rotation.x()+x;
    qreal roty=m_rotation.y()+y;
    qreal rotz=m_rotation.z()+z;
    this->setRotation(rotx, roty, rotz, preventHeadOver);
}

/** \brief set camera´s translation offset

**/
void Camera::setTranslationOffset(qreal x, qreal y, qreal z)
{
    m_position.setX(m_position.x()+x);
    m_position.setY(m_position.y()+y);
    m_position.setZ(m_position.z()+z);
    //qDebug() << __PRETTY_FUNCTION__ << m_position << m_rotation;
}
