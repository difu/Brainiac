#ifndef MOTIONTREEACTION_H
#define MOTIONTREEACTION_H

class MotionTree;
class MotionTreeEditorItem;

class MotionTreeAction
{
public:
    MotionTreeAction(MotionTree *motionTree);
    virtual ~MotionTreeAction();

protected:
    MotionTree *m_motionTree;
    MotionTreeEditorItem *m_editorItem;
};

#endif // MOTIONTREEACTION_H
