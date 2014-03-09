#ifndef MOTIONTREEACTION_H
#define MOTIONTREEACTION_H

#include<Qt>
#include<QList>

class MotionTree;
class MotionTreeEditorItem;

class MotionTreeAction
{
public:
    MotionTreeAction(MotionTree *motionTree);
    void addTriggerId(quint32 id);
    MotionTreeEditorItem *getEditorItem() const { return m_editorItem; }
    MotionTree * getMotionTree() const { return m_motionTree; }
    QString getName();
    QList<quint32> getTriggerIds() const { return m_triggerIds; }
    bool isTreeDefaultAction() const;
    void setPhase(qreal phase);
    virtual ~MotionTreeAction();

protected:
    MotionTree *m_motionTree;
    MotionTreeEditorItem *m_editorItem;
    QList<quint32> m_triggerIds;
};

#endif // MOTIONTREEACTION_H
