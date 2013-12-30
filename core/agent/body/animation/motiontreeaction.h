#ifndef MOTIONTREEACTION_H
#define MOTIONTREEACTION_H

class MotionTree;

class MotionTreeAction
{
public:
    MotionTreeAction(MotionTree *motionTree);
    virtual ~MotionTreeAction();

protected:
    MotionTree *m_motionTree;
};

#endif // MOTIONTREEACTION_H
