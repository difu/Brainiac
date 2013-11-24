#include "bodydisplay_.h"

#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/body/bodymanager.h"
#include "core/agent/body/body.h"


BodyDisplay_::BodyDisplay_() : OsgMultithreadedViewerWidget()
{
//    osg::Camera* camera = createCamera( 50, 50, 640, 480 );
//    osg::Node *ground=osgDB::readNodeFile("/Users/dirkfuchs/Desktop/ground.obj");
    setWindowFlags(Qt::Tool);
    setWindowTitle("Body Editor");
    show();
}

BodyDisplay_::~BodyDisplay_() {
    qDebug() << __PRETTY_FUNCTION__;
}

void BodyDisplay_::setAgentManager(AgentManager *agentManager)
{
    m_agentManager=agentManager;
    m_rootNode->removeChildren(0,m_rootNode->getNumChildren());
    m_rootNode->addChild(m_agentManager->getBodyAgent()->getBody()->getBodyRoot());
}
