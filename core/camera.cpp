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
void Camera::moveCamera(qreal offset)
{
    setTranslationOffset(offset*BrainiacGlobals::sinGrad(-m_rotation.y()),offset*BrainiacGlobals::sinGrad(m_rotation.z()),offset*BrainiacGlobals::cosGrad(-m_rotation.y()));
}


/** \brief places camera in OpenGL context

**/
void Camera::placeCameraGL() {
    glRotatef(m_rotation.x(),1,0,0);
    glRotatef(m_rotation.y(),0,1,0);


    glTranslatef(-m_position.x(), -m_position.y(), -m_position.z());
}


/** \brief set camera´s rotation

**/
void Camera::setRotation(QVector3D rot)
{
    m_rotation=rot;
}

/** \brief set camera´s rotation

                 set the rotation of the camera, the angles are also normalized
**/
void Camera::setRotation(qreal x, qreal y, qreal z)
{
    BrainiacGlobals::normalizeAngle(&x);
    BrainiacGlobals::normalizeAngle(&y);
    BrainiacGlobals::normalizeAngle(&z);
    m_rotation.setX(x);
    m_rotation.setY(y);
    m_rotation.setZ(z);
}

/** \brief set camera´s rotation offset

                   the values are added and the angles are also normalized
**/
void Camera::setRotationOffset(qreal x, qreal y, qreal z)
{
    qreal rotx=m_rotation.x()+x;
    qreal roty=m_rotation.y()+y;
    qreal rotz=m_rotation.z()+z;
    this->setRotation(rotx, roty, rotz);
}

/** \brief set camera´s translation

**/
void Camera::setTranslationOffset(qreal x, qreal y, qreal z)
{
    m_position.setX(m_position.x()+x);
    m_position.setY(m_position.y()+y);
    m_position.setZ(m_position.z()+z);
}
