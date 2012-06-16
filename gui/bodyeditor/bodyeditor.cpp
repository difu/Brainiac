#include "bodyeditor.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"

BodyEditor::BodyEditor(Scene *scene, AgentManager *agentManager) : EditorBase(scene)
{
    m_agentManager=agentManager;
    foreach(SkeletonNode *n, agentManager->getMasterAgent()->getBody()->getAllSkeletonNodes() ) {
        qDebug() << __PRETTY_FUNCTION__ << "added node "<< n->objectName();
        qDebug() << __PRETTY_FUNCTION__ << agentManager->getEditorSkeletonNodeLocations().value(n->getId()).x();
    }
}
