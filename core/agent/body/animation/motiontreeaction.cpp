#include "motiontreeaction.h"

#include "gui/Animation/motiontreeeditoritem.h"
#include "core/agent/body/animation/motiontree.h"
#include "gui/Animation/motiontreeeditor.h"
#include "gui/Animation/motiontreeeditoritemconnector.h"

MotionTreeAction::MotionTreeAction(MotionTree *motionTree) : m_motionTree(motionTree)
{
    m_editorItem=new MotionTreeEditorItem(BrainiacGlobals::ACTION,this);
    m_motionTree->getMotionTreeEditor()->addItem(m_editorItem);

}

QString MotionTreeAction::getName()
{
    return m_motionTree->getActionName(this);
}

bool MotionTreeAction::isTreeDefaultAction() const
{
    if(m_motionTree->getDefaultAction()==this) {
        return true;
    }
    return false;
}

void MotionTreeAction::setPhase(qreal phase)
{
    QList<EditorItemConnector *> connectors=m_editorItem->getConnectors();
    if(connectors.count()!=2) {
        qDebug() << __PRETTY_FUNCTION__ << "Expected 2 connectors!";
        return;
    }
    MotionTreeEditorItemConnector *c1=(MotionTreeEditorItemConnector *)connectors.at(0);
    MotionTreeEditorItemConnector *c2=(MotionTreeEditorItemConnector *)connectors.at(1);
    if(c1->startItem()==m_editorItem) {
        if(phase<=0.5) {
            c1->setProgress(phase*2.0);
        } else {
            c2->setProgress((phase-0.5)*2.0);
        }

    } else {
        if(phase<=0.5) {
            c2->setProgress(phase*2.0);
        } else {
            c1->setProgress((phase-0.5)*2.0);
        }
    }
}

MotionTreeAction::~MotionTreeAction()
{
    delete m_editorItem;
}
