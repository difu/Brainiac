#ifndef MOTIONTREETRANSITION_H
#define MOTIONTREETRANSITION_H

#include "core/brainiacglobals.h"

class MotionTree;
class MotionTreeEditorItem;

class MotionTreeTransition
{
public:
    MotionTreeTransition(MotionTree *tree);
    MotionTreeEditorItem *getEditorItem() const { return m_editorItem; }
    quint32 getColorId() { return m_color_id; }
    QColor getColor() { return BrainiacGlobals::EditorColors.at(m_color_id%BrainiacGlobals::EditorColors.count()) ;}
    void setColorId(quint32 colorId) { m_color_id=colorId%BrainiacGlobals::EditorColors.count(); }
    virtual ~MotionTreeTransition();

protected:
    MotionTree *m_motionTree;
    MotionTreeEditorItem *m_editorItem;
    quint32 m_color_id; //!< Id of color of this Transition
};

#endif // MOTIONTREETRANSITION_H
