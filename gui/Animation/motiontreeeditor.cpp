#include "motiontreeeditor.h"
#include "core/agent/body/animation/motiontree.h"

MotionTreeEditor::MotionTreeEditor(MotionTree *motionTree, Scene *scene) :
    EditorBase(scene), m_motionTree(motionTree)
{
}
