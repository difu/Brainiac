#include "bodysegment.h"
#include "core/agent/body/bodysegmentchannelhandler.h"
#include "core/agent/body/segmentshape.h"

BodySegment::BodySegment(SegmentShape *segmentShape):MatrixTransform()
{
    m_segmentShape=segmentShape;
    m_channelHandler=new BodySegmentChannelHandler(this);
    addChild(m_segmentShape->getGLSegment());
    m_matrixDirty=true;
    computeMatrix();
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
                m*=osg::Matrix::translate(m_segmentShape->getRestRotation().x(),0.0f,0.0f);
                break;
            case BrainiacGlobals::TY:
                m*=osg::Matrix::translate(0.0f,m_segmentShape->getRestRotation().y(),0.0f);
                break;
            case BrainiacGlobals::TZ:
                m*=osg::Matrix::translate(0.0f,0.0f,m_segmentShape->getRestRotation().z());
                break;
            }
        }
        this->setMatrix(m);
        m_matrixDirty=false;
    }
}

BodySegment::~BodySegment()
{
    m_channelHandler->deleteLater();
}
