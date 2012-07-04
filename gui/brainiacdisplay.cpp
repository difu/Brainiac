#include "brainiacdisplay.h"
#include <QMouseEvent>

BrainiacDisplay::BrainiacDisplay(QWidget *parent) :
    QGLView(parent)
{
    m_camera=new QGLCamera();
    setWindowFlags(Qt::Tool);
    setGeometry(this->geometry().x(),this->geometry().y(),600,400);
    m_camera->setFarPlane(1000000);
    setCamera(m_camera);
    setOption(QGLView::CameraNavigation,false);
}

void BrainiacDisplay::keyPressEvent(QKeyEvent *event)
{
    QGLView::keyPressEvent(event);
    if(event->key()==Qt::Key_Shift) {
        m_shiftPressed=true;
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

    } else if (event->buttons() & Qt::RightButton)  {
        if(m_shiftPressed) {
            m_camera->translateEye(-(qreal)0,0.0f,(qreal)dx);
            m_camera->translateCenter(-(qreal)0,0.0f,(qreal)dx);
            //qDebug() << __PRETTY_FUNCTION__ << m_camera->eye() << m_camera->center();
        } else {
            m_camera->translateCenter(-(qreal)dx/2.0f,(qreal)dy/2.0f,0.0f);
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
