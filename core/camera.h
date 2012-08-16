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


#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>

class Scene;
class QMouseEvent;
class Camera
{
public:
    Camera(Scene *scene, qreal xPos=0.0, qreal yPos=0.0, qreal zPos=0.0, qreal xRot=0.0, qreal yRot=0.0, qreal zRot=0.0);
    void moveCameraForward( qreal offset );
    void moveCameraSidewise( qreal offset );
    void placeCameraGL();
//    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
    void setRotation( QVector3D rot, bool preventHeadOver=true );
    void setRotation(qreal x, qreal y, qreal z, bool preventHeadOver=true);
    void setRotationOffset(qreal x, qreal y, qreal z , bool preventHeadOver=true);
    void setTranslationOffset(qreal x, qreal y, qreal z);

protected:
    Scene *m_scene; //!< the scene
    QVector3D m_position; //!< position of camera in world space
    QVector3D m_rotation; //!< rotation of camera

};

#endif // CAMERA_H
