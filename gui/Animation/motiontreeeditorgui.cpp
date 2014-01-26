#include "motiontreeeditorgui.h"
#include "ui_motiontreeeditorgui.h"

#include "core/agent/agentmanager.h"
#include "core/agent/body/animation/motiontreemanager.h"
#include "core/agent/body/animation/motiontree.h"

MotionTreeEditorGui::MotionTreeEditorGui(Scene *scene, QWidget *parent) :
    QWidget(parent),
    ItemEditorWidgetsBase(scene),
    ui(new Ui::MotionTreeEditorGui)
{
    ui->setupUi(this);
    //connect(ui->m_buttonTrack1,SIGNAL(clicked()),this,SLOT(motionTreeButtonPressed(quint32));
    connect(ui->m_buttonTrack1,SIGNAL(clicked()),this,SLOT(motionTreeButtonPressed()),Qt::DirectConnection);
    connect(ui->m_buttonTrack2,SIGNAL(clicked()),this,SLOT(motionTreeButtonPressed()),Qt::DirectConnection);
    connect(ui->m_buttonTrack3,SIGNAL(clicked()),this,SLOT(motionTreeButtonPressed()),Qt::DirectConnection);
    connect(ui->m_buttonTrack4,SIGNAL(clicked()),this,SLOT(motionTreeButtonPressed()),Qt::DirectConnection);
    connect(ui->m_buttonTrack5,SIGNAL(clicked()),this,SLOT(motionTreeButtonPressed()),Qt::DirectConnection);
    connect(ui->m_buttonTrack6,SIGNAL(clicked()),this,SLOT(motionTreeButtonPressed()),Qt::DirectConnection);
    connect(ui->m_buttonTrack7,SIGNAL(clicked()),this,SLOT(motionTreeButtonPressed()),Qt::DirectConnection);
    connect(ui->m_buttonTrack8,SIGNAL(clicked()),this,SLOT(motionTreeButtonPressed()),Qt::DirectConnection);

}

void MotionTreeEditorGui::motionTreeButtonPressed()
{
    quint32 id=0;
    if(ui->m_buttonTrack1->isChecked())
        id=0;
    else if(ui->m_buttonTrack2->isChecked())
        id=1;
    else if(ui->m_buttonTrack3->isChecked())
        id=2;
    else if(ui->m_buttonTrack4->isChecked())
        id=3;
    else if(ui->m_buttonTrack5->isChecked())
        id=4;
    else if(ui->m_buttonTrack6->isChecked())
        id=5;
    else if(ui->m_buttonTrack7->isChecked())
        id=6;
    else if(ui->m_buttonTrack8->isChecked())
        id=7;
    setActiveTree(id);

    emit motionTreeChange(id);
}

void MotionTreeEditorGui::setActiveTree(quint32 id)
{
    Q_ASSERT(id>=0 && id <=7);
    m_agentManager->setActiveMotionTreeEditor(id);
    ui->m_buttonTrack1->setChecked(id==0);
    ui->m_buttonTrack2->setChecked(id==1);
    ui->m_buttonTrack3->setChecked(id==2);
    ui->m_buttonTrack4->setChecked(id==3);
    ui->m_buttonTrack5->setChecked(id==4);
    ui->m_buttonTrack6->setChecked(id==5);
    ui->m_buttonTrack7->setChecked(id==6);
    ui->m_buttonTrack8->setChecked(id==7);
    ui->m_le_defaultAction->setText(m_agentManager->getMotionTreeManager()->getMotionTrees().value(m_agentManager->getActiveMotionTreeEditor())->getDefaultActionName());
}

void MotionTreeEditorGui::setAgentManager(AgentManager *agentManager)
{
    m_agentManager=agentManager;
    setActiveTree(m_agentManager->getActiveMotionTreeEditor());
}


MotionTreeEditorGui::~MotionTreeEditorGui()
{
    delete ui;
}
