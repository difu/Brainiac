#include "bodydisplay.h"

#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/body/bodymanager.h"
#include "core/agent/body/body.h"


BodyDisplay::BodyDisplay() : OsgMultithreadedViewerWidget()
{
//    osg::Camera* camera = createCamera( 50, 50, 640, 480 );
//    osg::Node *ground=osgDB::readNodeFile("/Users/dirkfuchs/Desktop/ground.obj");
    connect(getGlWindow()->getKeyMouseEater(),SIGNAL(keyPressed(Qt::Key)),this,SLOT(keyPressed(Qt::Key)));
    setWindowFlags(Qt::Tool);
    setWindowTitle("Body Editor");
    show();
}

BodyDisplay::~BodyDisplay() {
    qDebug() << __PRETTY_FUNCTION__;
}

void BodyDisplay::setAgentManager(AgentManager *agentManager)
{
    m_agentManager=agentManager;
    m_sceneNode->removeChildren(0,m_sceneNode->getNumChildren());
    if( m_agentManager)
        m_sceneNode->addChild(m_agentManager->getBodyAgent()->getBody()->getBodyRoot());
}

void BodyDisplay::keyPressed(Qt::Key key)
{
    if(key==BrainiacGlobals::ToggleSegmentCoordCrossesKey) {
        m_agentManager->getBodyAgent()->getBody()->toggleShowCoordCrosses();
    } else if(key==BrainiacGlobals::ToggleAgentPositionMarkerKey) {
        m_agentManager->getBodyAgent()->getBody()->toggleShowPositionMarker();
    }
}
