#include "actioneditor.h"
#include "ui_actioneditor.h"
#include "core/scene.h"
#include "core/simulation.h"
#include "core/group/group.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"
#include "core/agent/body/animation/animation.h"
#include "core/agent/body/animation/animationcurve.h"
#include "core/agent/body/animation/animationplayer.h"
#include "core/agent/body/animation/modifiableanimation.h"
#include "gui/actiondisplay.h"
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

    // Agent Tab UI
    connect(ui->pb_ApplyAgentCurves,SIGNAL(clicked()),this,SLOT(bakeAgentCurves()));
}

void ActionEditor::addCurvesToList(SkeletonNode *node, quint32 level)
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
        if( channelName == node->objectName()) {
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
    foreach(QGLSceneNode *n,node->children()) {
        SkeletonNode *sn=dynamic_cast<SkeletonNode *>(n);
        if(sn) {
            addCurvesToList(sn,level);
        }
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
    addCurvesToList(m_agentManager->getMasterAgent()->getBody()->getRootSkeletonNode(),0);
}

void ActionEditor::setActiveAnimation(quint32 animId)
{
    QMutexLocker locker(&m_animationChangeMutex);
    Q_UNUSED(locker);
    if(m_activeAnimation)
        delete m_activeAnimation;
    m_activeAnimation=new ModifiableAnimation(m_agentManager->getAnimations()->value(animId),m_agent->getBody());
    m_activeAnimationId=animId;
    ui->lineEditAnimationName->setText(m_activeAnimation->name());
    //m_activeAnimation=m_agentManager->getAnimations()->value(animId);
    refreshCurveList();
    m_loopEditorScene->setAnimation(m_activeAnimation);
    updateLoopUI();
}

void ActionEditor::setAgentManager(AgentManager *manager)
{
    m_agentManager=manager;
    if(m_agent) {
        delete m_agent;
    }
    m_activeAnimationId=0;
    m_agent=m_agentManager->cloneAgent(0);
    m_agent->setObjectName("ActionEditorAgent");
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
}

void ActionEditor::uiLoopTimesChanged()
{
    m_activeAnimation->setStartTime(ui->le_startTime->text().toDouble());
    m_activeAnimation->setEndTime(ui->le_endTime->text().toDouble());
    m_activeAnimation->setCrossFade(ui->le_crossFadeTime->text().toDouble());
    updateLoopUI();
}

void ActionEditor::uiLoopAnimModeStatic()
{
    m_activeAnimation->setAnimationType(BrainiacGlobals::STATIC);
    updateLoopUI();
}

void ActionEditor::uiLoopAnimModeRamp()
{
    m_activeAnimation->setAnimationType(BrainiacGlobals::RAMP);
    updateLoopUI();
}

void ActionEditor::uiLoopAnimModeLocomotion()
{
    m_activeAnimation->setAnimationType(BrainiacGlobals::LOCOMOTION);
    updateLoopUI();
}

void ActionEditor::uiLoopAnimModeTurning()
{
    m_activeAnimation->setAnimationType(BrainiacGlobals::TURNING);
    updateLoopUI();
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
    m_lastTabIndex=tabIndex;
}

ActionEditor::~ActionEditor()
{
    delete ui;
}
