#ifndef SCENEDISPLAY_H
#define SCENEDISPLAY_H

#include <QGLWidget>
#include <QCloseEvent>
#include <QVector3D>

class Camera;
class Scene;

/** \brief  Shows the Scene

        This class displays the OpenGL content of the scene
        It also provides information about mouse movements to navigate the camera
        @sa Camera

**/
class SceneDisplay : public QGLWidget
{
    Q_OBJECT
public:
    explicit SceneDisplay(Scene *scene, Camera *camera, QWidget *parent = 0);
    void setCamera(Camera *camera);

protected:
    void closeEvent(QCloseEvent *event);
    /** \brief  Paints information of each agent
            @sa QPainter painter

    **/
    void drawAgentsInfo(QPainter *painter);
    void initializeGL();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
    void paintGL();
    void resizeGL(int width, int height);
    Camera *m_camera; //!< Camera to display
    Scene *m_scene; //!< the scene
    QPoint m_lastPos; //!< last position of moouse to determine position delta
    bool m_shiftPressed; //!< true, if shift key is pressed
    QVector3D m_rotation; //!< Scene rotation
signals:


public slots:

};

#endif // SCENEDISPLAY_H
