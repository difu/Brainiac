#include "skeletongeometrynode.h"
#include "qglpainter.h"
#include "core/brainiacglobals.h"

SkeletonGeometryNode::SkeletonGeometryNode():QGLSceneNode()
{
    m_showSilhouette=false;
    setEffect(QGL::LitMaterial);
}

void SkeletonGeometryNode::drawGeometry(QGLPainter *painter)
{
    //painter->setStandardEffect(QGL::LitMaterial);
    painter->setFaceColor(QGL::AllFaces,m_color);
    painter->setColor(m_color);
    if(!m_showSilhouette) {
        QGLSceneNode::drawGeometry(painter);
    } else {
        QBox3D box=this->boundingBox();
        //QGLSceneNode::drawGeometry(painter);
        QVector3DArray vertices;

        qreal halfX=box.size().x()/2.0f;
        qreal halfY=box.size().y()/2.0f;
        qreal halfZ=box.size().z()/2.0f;

//        qreal crossLength=qMin(halfX,qMin(halfY,halfZ))*0.9f;

//        // Coord Cross Vertices
//        vertices.append(0,0,0);
//        vertices.append(crossLength,0,0);
//        vertices.append(0,0,0);
//        vertices.append(0,crossLength,0);
//        vertices.append(0,0,0);
//        vertices.append(0,0,crossLength);

        // Bounding Box Vertices

        vertices.append(box.center().x()-halfX,box.center().y()-halfY,box.center().z()-halfZ);
        vertices.append(box.center().x()+halfX,box.center().y()-halfY,box.center().z()-halfZ);
        vertices.append(box.center().x()+halfX,box.center().y()+halfY,box.center().z()-halfZ);
        vertices.append(box.center().x()-halfX,box.center().y()+halfY,box.center().z()-halfZ);

        vertices.append(box.center().x()-halfX,box.center().y()-halfY,box.center().z()+halfZ);
        vertices.append(box.center().x()+halfX,box.center().y()-halfY,box.center().z()+halfZ);
        vertices.append(box.center().x()+halfX,box.center().y()+halfY,box.center().z()+halfZ);
        vertices.append(box.center().x()-halfX,box.center().y()+halfY,box.center().z()+halfZ);

        vertices.append(box.center().x()-halfX,box.center().y()-halfY,box.center().z()-halfZ);
        vertices.append(box.center().x()-halfX,box.center().y()-halfY,box.center().z()+halfZ);

        vertices.append(box.center().x()+halfX,box.center().y()-halfY,box.center().z()-halfZ);
        vertices.append(box.center().x()+halfX,box.center().y()-halfY,box.center().z()+halfZ);

        vertices.append(box.center().x()+halfX,box.center().y()+halfY,box.center().z()-halfZ);
        vertices.append(box.center().x()+halfX,box.center().y()+halfY,box.center().z()+halfZ);

        vertices.append(box.center().x()-halfX,box.center().y()+halfY,box.center().z()-halfZ);
        vertices.append(box.center().x()-halfX,box.center().y()+halfY,box.center().z()+halfZ);

        painter->clearAttributes();
        painter->setStandardEffect(QGL::FlatColor);
        painter->setVertexAttribute(QGL::Position, vertices);

//        // Draw Coordinate Cross
//        painter->setColor(BrainiacGlobals::defaultXColor);
//        painter->draw(QGL::Lines, 2);
//        painter->setColor(BrainiacGlobals::defaultYColor);
//        painter->draw(QGL::Lines, 2,2);
//        painter->setColor(BrainiacGlobals::defaultZColor);
//        painter->draw(QGL::Lines, 2,4);

        // Draw bounding box
        painter->setColor(QColor(200,200,200));
        painter->draw(QGL::LineLoop,4,0);
        painter->draw(QGL::LineLoop,4,4);
        painter->draw(QGL::Lines, 2,8);
        painter->draw(QGL::Lines, 2,10);
        painter->draw(QGL::Lines, 2,12);
        painter->draw(QGL::Lines, 2,14);

    }
}

void SkeletonGeometryNode::showSilhouette(bool show)
{
     m_showSilhouette=show;
}
