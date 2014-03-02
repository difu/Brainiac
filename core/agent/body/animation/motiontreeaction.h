#ifndef MOTIONTREEACTION_H
#define MOTIONTREEACTION_H

#include<Qt>

class MotionTree;
class MotionTreeEditorItem;

class MotionTreeAction
{
public:
    MotionTreeAction(MotionTree *motionTree);
    MotionTreeEditorItem *getEditorItem() const { return m_editorItem; }
    void setPhase(qreal phase);
    virtual ~MotionTreeAction();

protected:
    MotionTree *m_motionTree;
    MotionTreeEditorItem *m_editorItem;
};

#endif // MOTIONTREEACTION_H
