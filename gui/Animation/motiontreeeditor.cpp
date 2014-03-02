#include "motiontreeeditor.h"
#include "core/agent/body/animation/motiontree.h"
#include <QWidget>

MotionTreeEditor::MotionTreeEditor(MotionTree *motionTree, Scene *scene) :
    EditorBase(scene), m_motionTree(motionTree)
{
}

