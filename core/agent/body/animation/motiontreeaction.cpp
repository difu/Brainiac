#include "motiontreeaction.h"

#include "gui/Animation/motiontreeeditoritem.h"
#include "core/agent/body/animation/motiontree.h"
#include "gui/Animation/motiontreeeditor.h"

MotionTreeAction::MotionTreeAction(MotionTree *motionTree) : m_motionTree(motionTree)
{
    m_editorItem=new MotionTreeEditorItem(BrainiacGlobals::ACTION,this);
    m_motionTree->getMotionTreeEditor()->addItem(m_editorItem);

}

MotionTreeAction::~MotionTreeAction()
{
    delete m_editorItem;
}
