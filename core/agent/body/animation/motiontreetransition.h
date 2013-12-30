#ifndef MOTIONTREETRANSITION_H
#define MOTIONTREETRANSITION_H

class MotionTree;

class MotionTreeTransition
{
public:
    MotionTreeTransition(MotionTree *tree);

protected:
    MotionTree *m_motionTree;
};

#endif // MOTIONTREETRANSITION_H
