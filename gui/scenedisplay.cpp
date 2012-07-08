#include "scenedisplay.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include "core/scene.h"
#include "core/simulation.h"
#include "core/camera.h"
#include "core/group/group.h"
#include <QMouseEvent>
#include <QDebug>
#include <glu.h>

#include <qglabstractscene.h>

#include "qglbuilder.h"
#include "qglcube.h"

#include "qglcamera.h"

SceneDisplay::SceneDisplay(Scene *scene, Camera *camera, QWidget *parent) :
    BrainiacDisplay(parent)
{
    m_scene=scene;
    m_shiftPressed=false;
    m_rotation=QVector3D(0,0,0);

    show();
    setWindowTitle("Scene");
    //m_camera->setNearPlane(100);

}

void SceneDisplay::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    //event->ignore();
}

void SceneDisplay::drawAgentsInfo(QPainter *painter)
{
//    GLdouble modelMatrix[16];
//    GLdouble projMatrix[16];
//    GLint viewport[4];

//    GLdouble pix_x;
//    GLdouble pix_y;
//    GLdouble pix_z;


//    int i=0;
//    foreach(Agent *agent, m_scene->getAgents()) {
//        QString info;
//        foreach(Group *grp,m_scene->getGroups()) {
//            if(grp->getAgents().contains(agent)) {
//                info.append(QString(grp->getName()));
//            }
//        }
//        info.append(".").append(QString::number(agent->getId()));
//        glMatrixMode(GL_MODELVIEW);
//        glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
//        glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
//        glGetIntegerv(GL_VIEWPORT, viewport);
//        gluProject(agent->getPosition()->x(), agent->getPosition()->y(), agent->getPosition()->z(),
//                   modelMatrix, projMatrix, viewport,
//                   &pix_x, &pix_y, &pix_z);
//        QRectF infoRect(pix_x,viewport[3] - (int)pix_y,100,50);
//        if(pix_z<=1.0f)
//            painter->drawText(infoRect,Qt::AlignLeft,info);
//    }
}


void SceneDisplay::initializeGL(QGLPainter *painter)
{

    m_rootSceneNode = new QGLSceneNode(0);
    foreach(Agent *agent, m_scene->getAgents()) {
        m_rootSceneNode->addNode((QGLSceneNode*)agent->getBody()->getRootSkeletonNode());
    }

    p_scene = QGLAbstractScene::loadScene(QLatin1String("/Users/dirkfuchs/Desktop/ground.obj"));
    QGLSceneNode *o = p_scene->mainNode();
    m_rootSceneNode->addNode(o);
    //qDumpScene(o);
    //o->draw(painter);
//    glShadeModel(GL_SMOOTH);
//    setAutoBufferSwap(true);
//    glClearColor(0.3f, 0.3f, 0.3f, 0.3f);
//    glClearDepth(1.0f);

//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_COLOR_MATERIAL);
}



void SceneDisplay::keyReleaseEvent(QKeyEvent *event)
{
    BrainiacDisplay::keyReleaseEvent(event);
}

//void SceneDisplay::mouseMoveEvent(QMouseEvent *event)
//{
////    int dx = event->x() - m_lastPos.x();
////    int dy = event->y() - m_lastPos.y();
////    if (event->buttons() & Qt::MidButton) {
////        if(m_camera) {
////            m_camera->setRotationOffset((qreal)-dy,(qreal)-dx,0.0f);
////        }
////        update();
////    } else if (event->buttons() & Qt::RightButton)  {
////        if(m_camera) {
////            if(!m_shiftPressed)
////                m_camera->moveCameraForward((qreal)-dx);
////            else {
////                m_camera->setTranslationOffset(0,dy,0);
////                m_camera->moveCameraSidewise((qreal)-dx);
////            }
////        }
////        update();
////    }
////    m_lastPos = event->pos();
//}

void SceneDisplay::mouseMoveEvent(QMouseEvent *event)
{
    BrainiacDisplay::mouseMoveEvent(event);
}

void SceneDisplay::mousePressEvent(QMouseEvent *event)
{
    BrainiacDisplay::mousePressEvent(event);
}

void SceneDisplay::paintGL(QGLPainter *painter)
{
    painter->setStandardEffect(QGL::LitMaterial);
    //painter->setFaceColor(QGL::AllFaces, QColor(170, 202, 0));
    m_rootSceneNode->draw(painter);


}

void SceneDisplay::resizeGL(int width, int height)
{
//    height = height?height:1;

//    glViewport( 0, 0, (GLint)width, (GLint)height );

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,50000.0f);

//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
}
