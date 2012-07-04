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
#include "gui/actiondisplay.h"
#include <QListWidgetItem>
#include <QHashIterator>

ActionEditor::ActionEditor(Scene *scene, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActionEditor)
{
    m_scene=scene;
    ui->setupUi(this);
    setWindowFlags(Qt::Tool);
    m_activeAnimation=0;
    //setWindowModality(Qt::Tool);
    connect(ui->listAnimation,SIGNAL(currentRowChanged(int)),this,SLOT(animationSelectionChanged(int)));
    m_actionDisplay=new ActionDisplay(this);
    startTimer(1000/m_scene->getSimulation()->getFps());
}

void ActionEditor::addCurvesToList(SkeletonNode *node, quint32 level)
{
    QHashIterator<QString, AnimationCurve*> i(m_activeAnimation->curves());
    while(i.hasNext()) {
        i.next();
        QString completeName=i.key();
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

void ActionEditor::animationSelectionChanged(int rowId)
{
    if(rowId >=0) {
        setActiveAnimation(ui->listAnimation->item(rowId)->data(Qt::UserRole).toUInt());
    }
}

void ActionEditor::setActiveAnimation(quint32 animId)
{
    m_activeAnimation=m_agentManager->getAnimations()->value(animId);
    addCurvesToList(m_agentManager->getMasterAgent()->getBody()->getRootSkeletonNode(),0);
}

void ActionEditor::setAgentManager(AgentManager *manager)
{
    m_agentManager=manager;
    qDebug() << __PRETTY_FUNCTION__ << "Active AgentManager" << manager->getGroup()->getName();
    ui->listAnimation->clear();
    ui->listCurves->clear();
    ui->tabWidgetMain->setTabEnabled(0,true);
        m_agentManager->loadAnimation("/Users/dirkfuchs/Programming/BrainiacNG/tmpTestData/newBodyFormatAction/Male1_A2_Sway.bvh");
    QHashIterator<quint32,Animation *> i(*m_agentManager->getAnimations()) ;

    while(i.hasNext()) {
        i.next();
        QListWidgetItem *animItem=new QListWidgetItem(ui->listAnimation);
//        qDebug() << __PRETTY_FUNCTION__ << "Key:" << i.key();
        animItem->setData(Qt::UserRole,QVariant(i.key()));
        animItem->setText(i.value()->name());
    }
    if(ui->listAnimation->count()) {
        ui->listAnimation->setCurrentRow(0);
    } else {
    }
    m_actionDisplay->setAgentManager(m_agentManager);

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
}

void ActionEditor::timerEvent(QTimerEvent *)
{
    static qreal time=0;
    time=time+1.0f/(qreal)m_scene->getSimulation()->getFps();
    if(time>10) time=0;
    if(m_activeAnimation&&this->isVisible()) {
        m_agentManager->getMasterAgent()->getBody()->getAnimationPlayer()->apply(*m_activeAnimation,time);
        m_actionDisplay->updateGL();
    }
}

ActionEditor::~ActionEditor()
{
    delete ui;
}
