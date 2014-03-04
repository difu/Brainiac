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
    MotionTree * getMotionTree() const { return m_motionTree; }
    QString getName();
    bool isTreeDefaultAction() const;
    void setPhase(qreal phase);
    virtual ~MotionTreeAction();

protected:
    MotionTree *m_motionTree;
    MotionTreeEditorItem *m_editorItem;
};

#endif // MOTIONTREEACTION_H
