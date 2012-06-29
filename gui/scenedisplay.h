#ifndef SCENEDISPLAY_H
#define SCENEDISPLAY_H

#include <qglview.h>
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
