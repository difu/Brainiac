#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>

class Scene;
class QMouseEvent;
class Camera
{
public:
    Camera(Scene *scene, qreal xPos=0.0, qreal yPos=0.0, qreal zPos=0.0, qreal xRot=0.0, qreal yRot=0.0, qreal zRot=0.0);
    void moveCamera( qreal offset );
    void placeCameraGL();
//    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
    void setRotation( QVector3D rot );
    void setRotation(qreal x, qreal y, qreal z);
    void setRotationOffset(qreal x, qreal y, qreal z);
    void setTranslationOffset(qreal x, qreal y, qreal z);

protected:
    Scene *m_scene; //!< the scene
    QVector3D m_position; //!< position of camera in world space
    QVector3D m_rotation; //!< rotation of camera

};

#endif // CAMERA_H
