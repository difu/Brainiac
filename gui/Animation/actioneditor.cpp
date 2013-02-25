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


#include "actioneditor.h"
#include "ui_actioneditor.h"
#include "core/scene.h"
#include "core/simulation.h"
#include "core/group/group.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/channel.h"
#include "core/agent/body/body.h"
#include "core/agent/body/bodymanager.h"
#include "core/agent/body/segment.h"
#include "core/agent/body/skeletonnode.h"
#include "core/agent/body/animation/animation.h"
#include "core/agent/body/animation/animationcurve.h"
#include "core/agent/body/animation/animationplayer.h"
#include "core/agent/body/animation/modifiableanimation.h"
#include "gui/actiondisplay.h"
#include "gui/brainiacslider.h"
#include "gui/Animation/loopeditorscene.h"
#include <QListWidgetItem>
#include <QHashIterator>
#include <QDoubleValidator>

ActionEditor::ActionEditor(Scene *scene, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActionEditor)
{
    m_scene=scene;
    ui->setupUi(this);
    setWindowFlags(Qt::Tool);
    m_activeAnimation=0;
    m_activeAnimationId=0;
    //setWindowModality(Qt::Tool);
    connect(ui->listAnimation,SIGNAL(currentRowChanged(int)),this,SLOT(animationSelectionChanged(int)));
    m_actionDisplay=new ActionDisplay(this);
    connect(m_actionDisplay,SIGNAL(animationOneFrameBackward()),this,SLOT(animationOneFrameBackward()));
    connect(m_actionDisplay,SIGNAL(animationOneFrameForward()),this,SLOT(animationOneFrameForward()));
    connect(m_actionDisplay,SIGNAL(animationRunningToggled()),this,SLOT(animationRunningToggle()));
    connect(ui->lineEditAnimationName,SIGNAL(returnPressed()),this,SLOT(animationNameChanged()));
    connect(ui->tabWidgetMain,SIGNAL(currentChanged(int)),this,SLOT(uiTabChanged(int)));
    startTimer(1000/m_scene->getSimulation()->getFps());
    m_agent=0;
    m_animationTime=0;
    m_animationRunning=true;

    // Curves Tab
    ui->listCurves->setSelectionMode(QAbstractItemView::MultiSelection);

    // Loop Tab UI
    m_loopEditorScene=new LoopEditorScene();
    ui->loopGraphicsView->setScene(m_loopEditorScene);
    ui->loopGraphicsView->show();
    m_doubleValidator=new QDoubleValidator();
    ui->le_crossFadeTime->setValidator(m_doubleValidator);
    ui->le_startTime->setValidator(m_doubleValidator);
    ui->le_endTime->setValidator(m_doubleValidator);
    connect(ui->le_startTime,SIGNAL(returnPressed()),this,SLOT(uiLoopTimesChanged()));
    connect(ui->le_endTime,SIGNAL(returnPressed()),this,SLOT(uiLoopTimesChanged()));
    connect(ui->le_crossFadeTime,SIGNAL(returnPressed()),this,SLOT(uiLoopTimesChanged()));
    connect(ui->pb_Locomotion,SIGNAL(clicked()),this,SLOT(uiLoopAnimModeLocomotion()));
    connect(ui->pb_Ramp,SIGNAL(clicked()),this,SLOT(uiLoopAnimModeRamp()));
    connect(ui->pb_Static,SIGNAL(clicked()),this,SLOT(uiLoopAnimModeStatic()));
    connect(ui->pb_Turning,SIGNAL(clicked()),this,SLOT(uiLoopAnimModeTurning()));
    connect(ui->pb_Apply,SIGNAL(clicked()),this,SLOT(bakeLoop()));
    connect(ui->pb_oneShot,SIGNAL(clicked()),this,SLOT(uiLoopOneShot()));
    connect(ui->pb_retrigger,SIGNAL(clicked()),this,SLOT(uiLoopRetrigger()));

    // Agent Tab UI
    connect(ui->pb_ApplyAgentCurves,SIGNAL(clicked()),this,SLOT(bakeAgentCurves()));

    // Transform Tab UI
    m_sliderTransformYRot=new BrainiacSlider(ui->gb_Rotation);
    m_sliderTransformYRot->setSliderColor(BrainiacGlobals::defaultYColor);
    m_sliderTransformYRot->setGeometry(10,70,930,40);
    m_sliderTransformYRot->setRange(-180.0f,180.0f);
    m_sliderTransformYRot->setText("Y");
    connect(m_sliderTransformYRot,SIGNAL(valueChanged(qreal)),this,SLOT(uiTranslationYRotChanged(qreal)));
}

void ActionEditor::addCurvesToList(const Segment &seg, quint32 level)
{
    //QHashIterator<QString, AnimationCurve*> i(m_activeAnimation->curves());
    foreach(QString curveName,m_activeAnimation->curveNames()) {
        if(!curveName.contains(':')) {
            continue;
        }
        QString completeName=curveName;
        QString channelName=completeName.split(':').at(0);
        QString channelType=completeName.split(':').at(1);

        //qDebug() << __PRETTY_FUNCTION__ << channelName << channelType << node->objectName() << level;
        if( channelName == seg.getName()) {
            QString prependedName=completeName;
            for(quint32 j=0;j<level;j++) {
                prependedName.prepend("    ");
            }
            QListWidgetItem *channelItem=new QListWidgetItem(ui->listCurves);
            channelItem->setData(Qt::UserRole,QVariant(completeName));
            channelItem->setText(prependedName);
            if(channelType.endsWith('x',Qt::CaseInsensitive))
                channelItem->setTextColor(BrainiacGlobals::defaultXColor);
            else if(channelType.endsWith('y',Qt::CaseInsensitive))
                channelItem->setTextColor(BrainiacGlobals::defaultYColor);
            else if(channelType.endsWith('z',Qt::CaseInsensitive))
                channelItem->setTextColor(BrainiacGlobals::defaultZColor);
        }

    }

    level++;
    foreach(quint32 childId ,m_agentManager->getBodyManager()->getSegmentChildrenIds(seg.getId())) {
        //SkeletonNode *sn=dynamic_cast<SkeletonNode *>(n);
        addCurvesToList(m_agentManager->getBodyManager()->getSegment(childId),level);

    }

}

void ActionEditor::animationNameChanged()
{
    if(m_activeAnimation) {
        m_activeAnimation->setName(ui->lineEditAnimationName->text());
        QListWidgetItem *item=ui->listAnimation->selectedItems().first();
        item->setText(m_activeAnimation->name());
    }
}

void ActionEditor::animationSelectionChanged(int rowId)
{
    if(rowId >=0) {
        setActiveAnimation(ui->listAnimation->item(rowId)->data(Qt::UserRole).toUInt());
    }
}

QString ActionEditor::getActiveAnimationName() const {
    if(m_activeAnimation) {
        return m_activeAnimation->name();
    } else return QString();
}

void ActionEditor::refreshCurveList()
{
    ui->listCurves->clear();
    foreach(QString curveName,m_activeAnimation->curveNames()) {
        if(!curveName.contains(':')) {
            QListWidgetItem *channelItem=new QListWidgetItem(ui->listCurves);
            channelItem->setData(Qt::UserRole,QVariant(curveName));
            channelItem->setText(curveName);
            if(curveName.compare("rx",Qt::CaseInsensitive)==0||curveName.compare("tx",Qt::CaseInsensitive)==0)
                channelItem->setTextColor(BrainiacGlobals::defaultXColor);
            else if(curveName.compare("ry",Qt::CaseInsensitive)==0||curveName.compare("ty",Qt::CaseInsensitive)==0)
                channelItem->setTextColor(BrainiacGlobals::defaultYColor);
            else if(curveName.compare("rz",Qt::CaseInsensitive)==0||curveName.compare("tz",Qt::CaseInsensitive)==0)
                channelItem->setTextColor(BrainiacGlobals::defaultZColor);
        }
    }
    //addCurvesToList(m_agentManager->getMasterAgent()->getBody()->getRootSkeletonNode(),0);
    addCurvesToList(m_agentManager->getBodyManager()->getRootSegment(),0);
}

void ActionEditor::setActiveAnimation(quint32 animId)
{
    QMutexLocker locker(&m_animationChangeMutex);
    Q_UNUSED(locker);
    if(m_activeAnimation)
        delete m_activeAnimation;
    // unset agent transformation as they might be set by previous Animations and their curves which this animation does maybe not have
    m_agent->getOutputChannel("rx")->setValue(0);
    m_agent->getOutputChannel("ry")->setValue(0);
    m_agent->getOutputChannel("rz")->setValue(0);
    m_agent->getOutputChannel("tx")->setValue(0);
    m_agent->getOutputChannel("ty")->setValue(0);
    m_agent->getOutputChannel("tz")->setValue(0);

    m_activeAnimation=new ModifiableAnimation(m_agentManager->getAnimations()->value(animId),m_agentManager);
    m_activeAnimationId=animId;
    ui->lineEditAnimationName->setText(m_activeAnimation->name());
    //m_activeAnimation=m_agentManager->getAnimations()->value(animId);
    refreshCurveList();
    m_loopEditorScene->setAnimation(m_activeAnimation);
    updateLoopUI();
    m_sliderTransformYRot->setValue(0.0f);
}

void ActionEditor::setAgentManager(AgentManager *manager)
{
    m_agentManager=manager;
//    if(m_agent) {
//        delete m_agent;
//    }
    m_activeAnimationId=0;
//    m_agent=m_agentManager->getBodyAgent();
    Agent *bodyAgent=m_agentManager->getActionAgent();
    //m_agent=m_agentManager->cloneAgent(0);
    //m_agent->setObjectName("ActionEditorAgent");
    m_agent=bodyAgent;
    m_actionDisplay->setAgent(m_agent);
    qDebug() << __PRETTY_FUNCTION__ << "Active AgentManager" << manager->getGroup()->getName();
    ui->listAnimation->clear();
    ui->listCurves->clear();
    ui->tabWidgetMain->setTabEnabled(ACTIONS,true);
        //m_agentManager->loadAnimation("/Users/dirkfuchs/Programming/BrainiacNG/tmpTestData/newBodyFormatAction/Male1_B3_Walk.bvh");
    QHashIterator<quint32,Animation *> i(*m_agentManager->getAnimations()) ;

    while(i.hasNext()) {
        i.next();
        QListWidgetItem *animItem=new QListWidgetItem(ui->listAnimation);
//        qDebug() << __PRETTY_FUNCTION__ << "Key:" << i.key();
        animItem->setData(Qt::UserRole,QVariant(i.key()));
        animItem->setText(i.value()->name());
    }
    if(ui->listAnimation->count()) {
        //Enable all tabs
        ui->tabWidgetMain->setEnabled(true);
        ui->listAnimation->setCurrentRow(0);
    } else {
        // Disable all tabs
        ui->tabWidgetMain->setEnabled(false);
    }
    //m_actionDisplay->setAgentManager(m_agentManager);

}

void ActionEditor::hide()
{
    m_actionDisplay->hide();
    QDialog::hide();
}

void ActionEditor::show()
{
    QDialog::show();
    m_actionDisplay->show();
    ui->tabWidgetMain->setCurrentIndex(0);
}

void ActionEditor::animationOneFrameBackward()
{
    m_animationTime=m_animationTime-1.0f/(qreal)m_scene->getSimulation()->getFps();
}

void ActionEditor::animationOneFrameForward()
{
    m_animationTime=m_animationTime+1.0f/(qreal)m_scene->getSimulation()->getFps();
}

void ActionEditor::animationStart()
{
    m_animationRunning=true;
}

void ActionEditor::animationStop()
{
    m_animationRunning=false;
}

void ActionEditor::animationRunningToggle() {
    m_animationRunning=!m_animationRunning;
}

void ActionEditor::applyAnimation()
{
    if(m_activeAnimation) {

        if(m_animationTime>m_activeAnimation->getEndTime())
            m_animationTime=m_activeAnimation->getStartTime();
        else if(m_animationTime<m_activeAnimation->getStartTime())
            m_animationTime=m_activeAnimation->getEndTime();

        if(this->isVisible()) {
            m_agent->getBody()->getAnimationPlayer()->apply(*m_activeAnimation,m_animationTime);
            if(!m_animationRunning) {

                    m_agent->getOutputChannel("rx")->setValue(0);
                    m_agent->getOutputChannel("ry")->setValue(0);
                    m_agent->getOutputChannel("rz")->setValue(0);
                    m_agent->getOutputChannel("tx")->setValue(0);
                    m_agent->getOutputChannel("ty")->setValue(0);
                    m_agent->getOutputChannel("tz")->setValue(0);

            }
            m_agent->advance(Agent::NONE);
            m_agent->advanceCommit();
            m_actionDisplay->setCameraOffset(m_activeAnimation->getRootBoneTranslation(m_animationTime));
            m_actionDisplay->updateGL();
            m_loopEditorScene->updateTime(m_animationTime);
        }
    }
}

void ActionEditor::bakeAgentCurves()
{
    m_activeAnimation->bake();
    refreshCurveList();
}

void ActionEditor::bakeLoop()
{
    m_activeAnimation->bake();
    updateLoopUI();
}

void ActionEditor::timerEvent(QTimerEvent *)
{
    QMutexLocker locker(&m_animationChangeMutex);
    Q_UNUSED(locker);
    if(m_animationRunning) {
        animationOneFrameForward();
    }
    ui->label_CurrentTime->setText(QString::number(m_animationTime));
    applyAnimation();
}

void ActionEditor::updateLoopUI()
{
    ui->le_crossFadeTime->setText(QString::number(m_activeAnimation->getCrossFade()));
    ui->le_startTime->setText(QString::number(m_activeAnimation->getStartTime()));
    ui->le_endTime->setText(QString::number(m_activeAnimation->getEndTime()));
    m_loopEditorScene->update();
    ui->pb_Locomotion->setChecked(m_activeAnimation->animationType()==BrainiacGlobals::LOCOMOTION);
    ui->pb_Static->setChecked(m_activeAnimation->animationType()==BrainiacGlobals::STATIC);
    ui->pb_Ramp->setChecked(m_activeAnimation->animationType()==BrainiacGlobals::RAMP);
    ui->pb_Turning->setChecked(m_activeAnimation->animationType()==BrainiacGlobals::TURNING);
    ui->tb_loopRx->setChecked(m_activeAnimation->crossFadeRx());
    ui->tb_loopRy->setChecked(m_activeAnimation->crossFadeRy());
    ui->tb_loopRz->setChecked(m_activeAnimation->crossFadeRz());
    ui->tb_loopTx->setChecked(m_activeAnimation->crossFadeTx());
    ui->tb_loopTy->setChecked(m_activeAnimation->crossFadeTy());
    ui->tb_loopTz->setChecked(m_activeAnimation->crossFadeTz());
    ui->pb_oneShot->setChecked(!m_activeAnimation->isLoopedAnimation());
    ui->pb_retrigger->setChecked(m_activeAnimation->isRetriggerable());
}

void ActionEditor::uiLoopTimesChanged()
{
    m_activeAnimation->setStartTime(ui->le_startTime->text().toDouble());
    m_activeAnimation->setEndTime(ui->le_endTime->text().toDouble());
    m_activeAnimation->setCrossFade(ui->le_crossFadeTime->text().toDouble());
    updateLoopUI();
    m_loopEditorScene->updateCurves();
}

void ActionEditor::uiLoopAnimModeStatic()
{
    m_activeAnimation->setAnimationType(BrainiacGlobals::STATIC);
    updateLoopUI();
    m_loopEditorScene->updateCurves();
}

void ActionEditor::uiLoopAnimModeRamp()
{
    m_activeAnimation->setAnimationType(BrainiacGlobals::RAMP);
    updateLoopUI();
    m_loopEditorScene->updateCurves();
}

void ActionEditor::uiLoopAnimModeLocomotion()
{
    m_activeAnimation->setAnimationType(BrainiacGlobals::LOCOMOTION);
    updateLoopUI();
     m_loopEditorScene->updateCurves();
}

void ActionEditor::uiLoopAnimModeTurning()
{
    m_activeAnimation->setAnimationType(BrainiacGlobals::TURNING);
    updateLoopUI();
    m_loopEditorScene->updateCurves();
}

void ActionEditor::uiLoopRetrigger()
{
    m_activeAnimation->setIsRetriggerable(ui->pb_retrigger->isChecked());
    updateLoopUI();
}

void ActionEditor::uiLoopOneShot()
{
    m_activeAnimation->setIsLoopedAnimation(!ui->pb_oneShot->isChecked());
    updateLoopUI();
}

void ActionEditor::uiTranslationYRotChanged(qreal value)
{
    m_activeAnimation->setTansformRotation(value);
}

void ActionEditor::uiTabChanged(int tabIndex)
{
    if(tabIndex==AGENT) {
        m_activeAnimation->createAgentCurves();
    } else {
        if(m_lastTabIndex==AGENT) {
            m_activeAnimation->resetAgentCurves();
        }
    }
    if(tabIndex==LOOP || tabIndex==EDIT) {
        QList<QString> selectedCurveNames;
        foreach(QListWidgetItem *item,ui->listCurves->selectedItems()){
            QString curveName=item->data(Qt::UserRole).toString();
            qDebug() << __PRETTY_FUNCTION__ << curveName;
            selectedCurveNames.append(curveName);
        }
        m_loopEditorScene->setAnimationCurveNames(selectedCurveNames);

    }
    m_lastTabIndex=tabIndex;
}

ActionEditor::~ActionEditor()
{
    delete ui;
}
