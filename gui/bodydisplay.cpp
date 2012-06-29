#include "bodydisplay.h"
#include "core/scene.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"

#include <QKeyEvent>

BodyDisplay::BodyDisplay(Scene *scene): BrainiacDisplay(0)
{
    m_scene=scene;
    m_rootSkeletonNode=0;
    m_agentManager=0;
    m_renderSilhouettes=false;
    m_renderSkeleton=false;
//    setWindowFlags(Qt::Tool);
//    setGeometry(this->geometry().x(),this->geometry().y(),600,400);
   show();
}

void BodyDisplay::setAgentManager(AgentManager *manager)
{
    m_agentManager=manager;
    if(m_agentManager) {
        m_rootSkeletonNode=m_agentManager->getMasterAgent()->getBody()->getRootSkeletonNode();
        qDebug() << m_agentManager->getName() << "in BodyDisplay";
    }
}

void BodyDisplay::keyPressEvent(QKeyEvent *e)
{
    BrainiacDisplay::keyPressEvent(e);
    if(e->key()==Qt::Key_S) {
        if(!m_shiftPressed) {
            if(m_agentManager) {
                m_renderSilhouettes=!m_renderSilhouettes;
                m_agentManager->getMasterAgent()->getBody()->renderSilhouettes(m_renderSilhouettes);
                updateGL();
            }
        } else {
            m_renderSkeleton=!m_renderSkeleton;
            updateGL();
        }
    }
}

void BodyDisplay::paintGL(QGLPainter *painter)
{
    painter->setStandardEffect(QGL::LitMaterial);
    //painter->setFaceColor(QGL::AllFaces, QColor(170, 202, 0));
    //m_rootSceneNode->draw(painter);
    if(m_rootSkeletonNode) {
        m_rootSkeletonNode->draw(painter);
        if(m_renderSkeleton) {
            renderSkeleton(painter,m_rootSkeletonNode);
        }
    }
}

void BodyDisplay::renderSkeleton(QGLPainter *painter, SkeletonNode *node)
{
    //painter->modelViewMatrix().push();
    foreach(QGLSceneNode *n,node->children()) {
        SkeletonNode *skelChildNode=dynamic_cast<SkeletonNode *>(n);
        if(skelChildNode) {
            painter->modelViewMatrix().push();
            QVector3DArray verts;
            painter->modelViewMatrix()*=node->transform();
            if(node->hasPrimitive()) {
                QMatrix4x4 m = skelChildNode->transform();
                QVector3D endVertex=m.map(QVector3D(0,0,0));
                verts.append(QVector3D(0,0,0));
                verts.append(endVertex);
                painter->clearAttributes();
                painter->setStandardEffect(QGL::FlatColor);
                painter->setColor(QColor(170, 202, 0));


                painter->setVertexAttribute(QGL::Position, verts);
                painter->draw(QGL::Lines, 2);
            }
        //painter->clearAttributes();
            qDebug()<< node->objectName() << node->localTransform();
            renderSkeleton(painter,skelChildNode);
            painter->modelViewMatrix().pop();
        }
    }





}
