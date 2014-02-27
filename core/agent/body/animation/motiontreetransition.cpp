#include "motiontreetransition.h"

#include "gui/Animation/motiontreeeditoritem.h"
#include "core/agent/body/animation/motiontree.h"
#include "gui/Animation/motiontreeeditor.h"

MotionTreeTransition::MotionTreeTransition(MotionTree *tree): m_motionTree(tree)
{
    m_editorItem=new MotionTreeEditorItem(BrainiacGlobals::TRANSITION,this);
    m_motionTree->getMotionTreeEditor()->addItem(m_editorItem);
    m_color_id=0;
}

MotionTreeTransition::~MotionTreeTransition()
{
    delete m_editorItem;
}
