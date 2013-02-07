#include "bodysegment.h"
#include "core/agent/body/bodysegmentchannelhandler.h"
#include "core/agent/body/segmentshape.h"

BodySegment::BodySegment(Body *body, SegmentShape *segmentShape):MatrixTransform(),m_body(body),m_segmentShape(segmentShape)
{
    m_channelHandler=new BodySegmentChannelHandler(this);
    addChild(m_segmentShape->getGLSegment());
    m_matrixDirty=true;
    computeMatrix();
    setName(m_segmentShape->getName().toStdString());
}

void BodySegment::computeMatrix() {
    if(m_matrixDirty) {
        osg::Matrix m;
        foreach(BrainiacGlobals::RotTrans rotTrans,m_segmentShape->getRotationTranslationOrder()) {
            switch(rotTrans) {
            case BrainiacGlobals::RX:
                //m.rotate()
                break;
            case BrainiacGlobals::RY:
                break;
            case BrainiacGlobals::RZ:
                break;
            case BrainiacGlobals::TX:
                m*=osg::Matrix::translate(m_segmentShape->getRestTranslation().x(),0.0f,0.0f);
                break;
            case BrainiacGlobals::TY:
                m*=osg::Matrix::translate(0.0f,m_segmentShape->getRestTranslation().y(),0.0f);
                break;
            case BrainiacGlobals::TZ:
                m*=osg::Matrix::translate(0.0f,0.0f,m_segmentShape->getRestTranslation().z());
                break;
            }
        }
        this->setMatrix(m);
        //this->setMatrix(osg::Matrix::translate(25.0f, 0.0f, 0.0f ));
        m_matrixDirty=false;
    }
}

BodySegment::~BodySegment()
{
    m_channelHandler->deleteLater();
}
