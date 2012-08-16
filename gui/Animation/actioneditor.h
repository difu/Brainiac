// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#ifndef ACTIONEDITOR_H
#define ACTIONEDITOR_H

#include <QDialog>
#include <QMutex>

class Agent;
class AgentManager;
class Animation;
class BrainiacSlider;
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
    void refreshCurveList();
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
    BrainiacSlider *m_sliderTransformYRot;
    /**
     * @brief index of tabs
     *
     * @enum Tabs
     */
    enum Tabs{ACTIONS=0,CURVES=1,EDIT=2,LOOP=3,AGENT=4};
    int m_lastTabIndex;
protected slots:
    void animationSelectionChanged(int rowId);
    void animationNameChanged();
    void uiTabChanged(int tabIndex);
    // Loop Tab Stuff
    void uiLoopTimesChanged();
    void uiLoopAnimModeLocomotion();
    void uiLoopAnimModeStatic();
    void uiLoopAnimModeTurning();
    void uiLoopAnimModeRamp();
    void uiLoopOneShot();
    void uiLoopRetrigger();
    void bakeLoop();
    void bakeAgentCurves();

    // Action Tab Stuff
    void uiTranslationYRotChanged(qreal value);
private:
    Ui::ActionEditor *ui;
};

#endif // ACTIONEDITOR_H
