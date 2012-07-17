#ifndef ACTIONEDITOR_H
#define ACTIONEDITOR_H

#include <QDialog>
#include <QMutex>

class Agent;
class AgentManager;
class Animation;
class LoopEditorScene;
class ModifiableAnimation;
class SkeletonNode;
class ActionDisplay;
class Scene;

class QDoubleValidator;

namespace Ui {
class ActionEditor;
}

/**
 * @brief the ActionEditor to edit the actions (Animation) of an Agent
 *
 * @class ActionEditor actioneditor.h "gui/Animation/actioneditor.h"
 */
class ActionEditor : public QDialog
{
    Q_OBJECT
    
public:
    explicit ActionEditor(Scene *scene, QWidget *parent = 0);
    void setAgentManager(AgentManager *manager);
    QString getActiveAnimationName() const;
    quint32 getActiveAnimationId() const {return m_activeAnimationId;}
    ~ActionEditor();

public slots:
    void hide();
    void show();
    void animationStart();
    void animationStop();
    void animationRunningToggle();
    void animationOneFrameForward();
    void animationOneFrameBackward();
protected:
    void applyAnimation();
    void setActiveAnimation(quint32 animId);
    void addCurvesToList(SkeletonNode *node, quint32 level);
    void updateLoopUI();
    void timerEvent(QTimerEvent *);
    AgentManager *m_agentManager;
    Agent *m_agent;
    ModifiableAnimation *m_activeAnimation;
    quint32 m_activeAnimationId;
    ActionDisplay *m_actionDisplay; /**< TODO */
    Scene *m_scene;
    QMutex m_animationChangeMutex;
    LoopEditorScene *m_loopEditorScene;
    QDoubleValidator *m_doubleValidator;
    qreal m_animationTime;
    bool m_animationRunning;
protected slots:
    void animationSelectionChanged(int rowId);
    void animationNameChanged();
    // Loop Tab Stuff
    void uiLoopTimesChanged();
    void uiLoopAnimModeLocomotion();
    void uiLoopAnimModeStatic();
    void uiLoopAnimModeTurning();
    void uiLoopAnimModeRamp();
private:
    Ui::ActionEditor *ui;
};

#endif // ACTIONEDITOR_H
