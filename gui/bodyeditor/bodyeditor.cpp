#include "bodyeditor.h"
#include "core/agent/agent.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"
#include "core/agent/body/skeletonnodesphere.h"
#include "core/agent/body/skeletonnodebox.h"
#include "core/brainiacglobals.h"
#include "gui/bodyeditor/bodyeditoritem.h"
#include "gui/editoritemconnector.h"

BodyEditor::BodyEditor(Scene *scene, AgentManager *agentManager) : EditorBase(scene)
{
    m_agentManager=agentManager;
    foreach(SkeletonNode *n, agentManager->getMasterAgent()->getBody()->getAllSkeletonNodes() ) {
        qDebug() << __PRETTY_FUNCTION__ << "added node "<< n->objectName();
        qDebug() << __PRETTY_FUNCTION__ << agentManager->getEditorSkeletonNodeLocations().value(n->getId()).x();
        if(dynamic_cast<SkeletonNodeBox*>(n)) {
            BodyEditorItem *item=new BodyEditorItem(BrainiacGlobals::CUBE,m_agentManager,n->getId());
            item->setPos(agentManager->getEditorSkeletonNodeLocations().value(n->getId()).x(),agentManager->getEditorSkeletonNodeLocations().value(n->getId()).y());
            addItem(item);
        } else if(dynamic_cast<SkeletonNodeSphere*>(n)) {
            BodyEditorItem *item=new BodyEditorItem(BrainiacGlobals::SPHERE,m_agentManager,n->getId());
            item->setPos(agentManager->getEditorSkeletonNodeLocations().value(n->getId()).x(),agentManager->getEditorSkeletonNodeLocations().value(n->getId()).y());
            addItem(item);
        }
    }
    foreach (QGraphicsItem *item, items()) {
        if(item->type()==EditorItem::Type) {
            EditorItem *parentEditorItem = qgraphicsitem_cast<EditorItem*> (item);
            AgentManager *parentManager=(AgentManager *)parentEditorItem->getObject();
            foreach(QGLSceneNode *childNode, parentManager->getMasterAgent()->getBody()->getSkeletonNodeById(parentEditorItem->getId())->children()) {
                SkeletonNode *skelChildNode=dynamic_cast<SkeletonNode *>(childNode);
                if(skelChildNode) {
                    foreach (QGraphicsItem *childItem, items()) {
                        if(childItem->type()==EditorItem::Type) {
                            EditorItem *childEditorItem = qgraphicsitem_cast<EditorItem*> (childItem);
                            if(childEditorItem->getId() == skelChildNode->getId()) {
                                EditorItemConnector *connector=new EditorItemConnector(parentEditorItem,childEditorItem);
                                addItem(connector);
                            }
                        }
                    }
                }
            }
        }
    }
    //! \bug  OK..Doing Here really nasty stuff. The Connectors are NOT SHOWING when scene is initially displayed So move all items one to the right and back again to schedule a redraw... Why the F*CK
    foreach (QGraphicsItem *item, items()) {
        if (item->type() == BodyEditorItem::Type) {
            BodyEditorItem *eItem=(BodyEditorItem *)item;
            eItem->setPos(eItem->x()+EditorItem::_raster,eItem->y());
            eItem->setPos(eItem->x()-EditorItem::_raster,eItem->y());
        }
    }
}

void BodyEditor::updateItemLocations()
{
    foreach (QGraphicsItem *item, items()) {
        if (item->type() == BodyEditorItem::Type) {
            BodyEditorItem* bItem=(BodyEditorItem *) item;
            m_agentManager->setBodyEditorTranslation(bItem->getId(),(qint32)item->pos().x(),(qint32)item->pos().y());
        }
    }
}
