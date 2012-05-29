#include "scenedisplay.h"
#include "core/agent/agent.h"
#include "core/scene.h"
#include "core/simulation.h"
#include "core/camera.h"
#include "core/group/group.h"
#include <QMouseEvent>
#include <QDebug>
#include <glu.h>

SceneDisplay::SceneDisplay(Scene *scene, Camera *camera, QWidget *parent) :
    QGLWidget(parent)
{
    m_scene=scene;
    m_camera=camera;
    setWindowFlags(Qt::Tool);
    setGeometry(this->geometry().x(),this->geometry().y(),600,400);
    show();
    m_shiftPressed=false;
    m_rotation=QVector3D(0,0,0);
}

void SceneDisplay::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    //event->ignore();
}

void SceneDisplay::drawAgentsInfo(QPainter *painter)
{
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint viewport[4];

    GLdouble pix_x;
    GLdouble pix_y;
    GLdouble pix_z;


    int i=0;
    foreach(Agent *agent, m_scene->getAgents()) {
        QString info;
        foreach(Group *grp,m_scene->getGroups()) {
            if(grp->getAgents().contains(agent)) {
                info.append(QString(grp->getName()));
            }
        }
        info.append(".").append(QString::number(agent->getId()));
        glMatrixMode(GL_MODELVIEW);
        glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
        glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
        glGetIntegerv(GL_VIEWPORT, viewport);
        gluProject(agent->getPosition()->x(), agent->getPosition()->y(), agent->getPosition()->z(),
                   modelMatrix, projMatrix, viewport,
                   &pix_x, &pix_y, &pix_z);
        QRectF infoRect(pix_x,viewport[3] - (int)pix_y,100,50);
        if(pix_z<=1.0f)
            painter->drawText(infoRect,Qt::AlignLeft,info);
    }
}

void SceneDisplay::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    setAutoBufferSwap(true);
    glClearColor(0.3f, 0.3f, 0.3f, 0.3f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}

void SceneDisplay::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Shift) {
        m_shiftPressed=true;
    }
}

void SceneDisplay::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Shift) {
        m_shiftPressed=false;
    }
}

void SceneDisplay::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();
    if (event->buttons() & Qt::MidButton) {
        if(m_camera) {
            m_camera->setRotationOffset((qreal)-dy,(qreal)-dx,0.0f);
        }
        update();
    } else if (event->buttons() & Qt::RightButton)  {
        if(m_camera) {
            if(!m_shiftPressed)
                m_camera->moveCameraForward((qreal)-dx);
            else {
                m_camera->setTranslationOffset(0,dy,0);
                m_camera->moveCameraSidewise((qreal)-dx);
            }
        }
        update();
    }
    m_lastPos = event->pos();
}

void SceneDisplay::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void SceneDisplay::paintEvent(QPaintEvent *)
{
    makeCurrent();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // set the model transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // set the light position and attributes
    const GLfloat lightPosition[] = { 1.0f, -1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    const GLfloat lightColorAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightColorAmbient);
    const GLfloat lightColorDiffuse[] = { 0.52f, 0.5f, 0.5f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColorDiffuse);
    const GLfloat lightColorSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColorSpecular);

    // Camera
    if(m_camera) {
        m_camera->placeCameraGL();
    }

    // Just a simple reference

    glPushMatrix();
    //glRotatef(m_rotation.x(),1,0,0);
    //glRotatef(m_rotation.y(),0,1,0);
    //glTranslatef(0,0,0);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glColor3f(1.0f,0.0f,0.0f); // x RED
        glVertex3f( 0.0f, 0.0f, 0.0f);
        glVertex3f( 20.0f,  0.0f, 0.0f);// y GREEN
        glColor3f(0.0f,1.0f,0.0f); //
        glVertex3f(  0.0f,  0.0f, 0.0f);
        glVertex3f(  0.0f, 20.0f, 00.0f);
        glColor3f(0.0f,0.0f,1.0f); // z BLUE
        glVertex3f(  0.0f,  0.0f, 0.0f);
        glVertex3f(  0.0f, 0.0f, 20.0f);
    glEnd();
    glEnable(GL_DEPTH_TEST);
    glBegin(GL_QUADS);
        glColor3f(0.0f,0.3f,0.0f);
        glVertex3f( -1000.0f, -1.0f, -1000.0f);
        glVertex3f( -1000.0f,  -1.0f, 1000.0f);
        glVertex3f(  1000.0f,  -1.0f, 1000.0f);
        glVertex3f(  1000.0f, -1.0f, -1000.0f);
    glEnd(); //
    glEnable(GL_DEPTH_TEST);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        glColor3f(0.0f,0.5f,0.0f);
        glVertex3f( -1000.0f, -1.5f, -1000.0f);
        glVertex3f( -1000.0f, -1.5f, 1000.0f);
        glVertex3f(  1000.0f, -1.5f, 1000.0f);
        glVertex3f(  1000.0f, -1.5f, -1000.0f);
    glEnd();
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glColor3f(0.0f,0.5f,0.0f);
        glVertex3f( -1000.0f, -1.5f, 0.0f);
        glVertex3f( 1000.0f,  -1.5f, 0.0f);
        glVertex3f(  0.0f,  -1.5f, 1000.0f);
        glVertex3f(  0.0f, -1.5f, -1000.0f);
    glEnd();
    glPopMatrix();



    foreach(Agent *agent, m_scene->getAgents()) {
        agent->renderGL();
    }

    QPainter painter(this);
    drawAgentsInfo(&painter);

    QRectF rect(10,10,500,100);
    QString frameInfo(tr("Render FPS: "));
    frameInfo.append(QString().setNum(m_scene->getSimulation()->getFpsCalc(),'g',3));
    painter.drawText(rect,Qt::AlignLeft,frameInfo);
    //painter.end();
}

void SceneDisplay::paintGL()
{

}

void SceneDisplay::resizeGL(int width, int height)
{
    height = height?height:1;

    glViewport( 0, 0, (GLint)width, (GLint)height );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,50000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
