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


#ifndef SCENEDISPLAY_H
#define SCENEDISPLAY_H

#include <QCloseEvent>
#include <QVector3D>
#include "gui/brainiacdisplay.h"
class Camera;
class Scene;
class QGLAbstractScene;
class QGLSceneNode;
class QGLCamera;

/** \brief  Shows the Scene

        This class displays the OpenGL content of the scene
        It also provides information about mouse movements to navigate the camera
        @sa Camera

**/
class SceneDisplay : public BrainiacDisplay
{
    Q_OBJECT
public:
    explicit SceneDisplay(Scene *scene, Camera *camera, QWidget *parent = 0);
    //void setCamera(Camera *camera);

protected:
    void closeEvent(QCloseEvent *event);
    /** \brief  Paints information of each agent
            @sa QPainter painter

    **/
    void drawAgentsInfo(QPainter *painter);
    void initializeGL(QGLPainter *painter);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    //void paintGL();
    void paintGL(QGLPainter *painter);
    void resizeGL(int width, int height);

    Scene *m_scene; //!< the scene
    QVector3D m_rotation; //!< Scene rotation

    QGLAbstractScene *p_scene;
    QGLSceneNode *m_rootSceneNode;
signals:


public slots:

};

#endif // SCENEDISPLAY_H
