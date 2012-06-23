#include "segmenteditor.h"
#include "gui/brainiacslider.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include "core/agent/body/skeletonnode.h"
#include "core/agent/body/skeletonnodebox.h"
#include "core/agent/body/skeletonnodesphere.h"
#include "ui_segmenteditor.h"

SegmentEditor::SegmentEditor(Scene *scene, QWidget *parent) :
    QWidget(parent),
    ItemEditorWidgetsBase(scene),
    ui(new Ui::SegmentEditor)
{
    ui->setupUi(this);
    createRxSliders();
    createTxSliders();
    createRestRxSliders();
    createRestTxSliders();
}

void SegmentEditor::createRestRxSliders()
{
    m_SliderRestRx=new BrainiacSlider(ui->frameShapeRestRots);
    m_SliderRestRy=new BrainiacSlider(ui->frameShapeRestRots);
    m_SliderRestRz=new BrainiacSlider(ui->frameShapeRestRots);

    m_SliderRestRx->setSliderColor(BrainiacGlobals::defaultXColor);
    m_SliderRestRy->setSliderColor(BrainiacGlobals::defaultYColor);
    m_SliderRestRz->setSliderColor(BrainiacGlobals::defaultZColor);

    m_SliderRestRx->setGeometry(0,2,300,40);
    m_SliderRestRy->setGeometry(0,35,300,40);
    m_SliderRestRz->setGeometry(0,69,300,40);

    m_SliderRestRx->setText("rx");
    m_SliderRestRy->setText("ry");
    m_SliderRestRz->setText("rz");

    m_SliderRestRx->setRange(-180.0f,180.0f);
    m_SliderRestRy->setRange(-180.0f,180.0f);
    m_SliderRestRz->setRange(-180.0f,180.0f);

    connect(m_SliderRestRx,SIGNAL(valueChanged(qreal)),this,SLOT(manualRestRotChanged(qreal)));
    connect(m_SliderRestRy,SIGNAL(valueChanged(qreal)),this,SLOT(manualRestRotChanged(qreal)));
    connect(m_SliderRestRz,SIGNAL(valueChanged(qreal)),this,SLOT(manualRestRotChanged(qreal)));
}

void SegmentEditor::createRestTxSliders()
{
    m_SliderRestTx=new BrainiacSlider(ui->frameShapeRestTrans);
    m_SliderRestTy=new BrainiacSlider(ui->frameShapeRestTrans);
    m_SliderRestTz=new BrainiacSlider(ui->frameShapeRestTrans);

    m_SliderRestTx->setSliderColor(BrainiacGlobals::defaultXColor);
    m_SliderRestTy->setSliderColor(BrainiacGlobals::defaultYColor);
    m_SliderRestTz->setSliderColor(BrainiacGlobals::defaultZColor);

    m_SliderRestTx->setGeometry(0,2,300,40);
    m_SliderRestTy->setGeometry(0,35,300,40);
    m_SliderRestTz->setGeometry(0,69,300,40);

    m_SliderRestTx->setText("tx");
    m_SliderRestTy->setText("ty");
    m_SliderRestTz->setText("tz");

    m_SliderRestTx->setRange(-180.0f,180.0f);
    m_SliderRestTy->setRange(-180.0f,180.0f);
    m_SliderRestTz->setRange(-180.0f,180.0f);

    connect(m_SliderRestTx,SIGNAL(valueChanged(qreal)),this,SLOT(manualRestTransChanged(qreal)));
    connect(m_SliderRestTy,SIGNAL(valueChanged(qreal)),this,SLOT(manualRestTransChanged(qreal)));
    connect(m_SliderRestTz,SIGNAL(valueChanged(qreal)),this,SLOT(manualRestTransChanged(qreal)));
}

void SegmentEditor::createRxSliders()
{
    m_SliderRx=new BrainiacSlider(ui->frameShapeRots);
    m_SliderRy=new BrainiacSlider(ui->frameShapeRots);
    m_SliderRz=new BrainiacSlider(ui->frameShapeRots);

    m_SliderRx->setSliderColor(BrainiacGlobals::defaultXColor);
    m_SliderRy->setSliderColor(BrainiacGlobals::defaultYColor);
    m_SliderRz->setSliderColor(BrainiacGlobals::defaultZColor);

    m_SliderRx->setGeometry(0,2,300,40);
    m_SliderRy->setGeometry(0,35,300,40);
    m_SliderRz->setGeometry(0,69,300,40);

    m_SliderRx->setText("rx");
    m_SliderRy->setText("ry");
    m_SliderRz->setText("rz");

    m_SliderRx->setRange(-180.0f,180.0f);
    m_SliderRy->setRange(-180.0f,180.0f);
    m_SliderRz->setRange(-180.0f,180.0f);

    connect(m_SliderRx,SIGNAL(valueChanged(qreal)),this,SLOT(manualRotChanged(qreal)));
    connect(m_SliderRy,SIGNAL(valueChanged(qreal)),this,SLOT(manualRotChanged(qreal)));
    connect(m_SliderRz,SIGNAL(valueChanged(qreal)),this,SLOT(manualRotChanged(qreal)));
}

void SegmentEditor::createTxSliders()
{
    m_SliderTx=new BrainiacSlider(ui->frameShapeTrans);
    m_SliderTy=new BrainiacSlider(ui->frameShapeTrans);
    m_SliderTz=new BrainiacSlider(ui->frameShapeTrans);

    m_SliderTx->setSliderColor(BrainiacGlobals::defaultXColor);
    m_SliderTy->setSliderColor(BrainiacGlobals::defaultYColor);
    m_SliderTz->setSliderColor(BrainiacGlobals::defaultZColor);

    m_SliderTx->setGeometry(0,2,300,40);
    m_SliderTy->setGeometry(0,35,300,40);
    m_SliderTz->setGeometry(0,69,300,40);

    m_SliderTx->setText("tx");
    m_SliderTy->setText("ty");
    m_SliderTz->setText("tz");

    m_SliderTx->setRange(-180.0f,180.0f);
    m_SliderTy->setRange(-180.0f,180.0f);
    m_SliderTz->setRange(-180.0f,180.0f);

    connect(m_SliderTx,SIGNAL(valueChanged(qreal)),this,SLOT(manualTransChanged(qreal)));
    connect(m_SliderTy,SIGNAL(valueChanged(qreal)),this,SLOT(manualTransChanged(qreal)));
    connect(m_SliderTz,SIGNAL(valueChanged(qreal)),this,SLOT(manualTransChanged(qreal)));
}

void SegmentEditor::manualRestRotChanged(qreal value)
{
    Q_UNUSED(value);
    m_agentManager->setSegmentRestRotation(m_id,m_SliderRestRx->getValue(),m_SliderRestRy->getValue(),m_SliderRestRz->getValue());
    emit updateGLContent();
    this->updateEditor();
}

void SegmentEditor::manualRestTransChanged(qreal value)
{
    Q_UNUSED(value);
    m_agentManager->setSegmentRestTranslation(m_id,m_SliderRestTx->getValue(),m_SliderRestTy->getValue(),m_SliderRestTz->getValue());
    emit updateGLContent();
    this->updateEditor();
}

void SegmentEditor::manualRotChanged(qreal value)
{
    Q_UNUSED(value);
    m_agentManager->setSegmentRotation(m_id,m_SliderRx->getValue(),m_SliderRy->getValue(),m_SliderRz->getValue());
    emit updateGLContent();
    this->updateEditor();
}

void SegmentEditor::manualTransChanged(qreal value)
{
    Q_UNUSED(value);
    m_agentManager->setSegmentTranslation(m_id,m_SliderTx->getValue(),m_SliderTy->getValue(),m_SliderTz->getValue());
    emit updateGLContent();
    this->updateEditor();
}

void SegmentEditor::setSegmentConfig(AgentManager *manager, quint32 id)
{
    m_agentManager=manager;
    m_id=id;
    updateEditor();
}

void SegmentEditor::updateEditor()
{
    m_SliderRx->setValue(m_agentManager->getMasterAgent()->getBody()->getSkeletonNodeById(m_id)->getRotation().x());
    m_SliderRy->setValue(m_agentManager->getMasterAgent()->getBody()->getSkeletonNodeById(m_id)->getRotation().y());
    m_SliderRz->setValue(m_agentManager->getMasterAgent()->getBody()->getSkeletonNodeById(m_id)->getRotation().z());

    m_SliderRestRx->setValue(m_agentManager->getMasterAgent()->getBody()->getSkeletonNodeById(m_id)->getRestRotation().x());
    m_SliderRestRy->setValue(m_agentManager->getMasterAgent()->getBody()->getSkeletonNodeById(m_id)->getRestRotation().y());
    m_SliderRestRz->setValue(m_agentManager->getMasterAgent()->getBody()->getSkeletonNodeById(m_id)->getRestRotation().z());

    m_SliderTx->setValue(m_agentManager->getMasterAgent()->getBody()->getSkeletonNodeById(m_id)->getTranslation().x());
    m_SliderTy->setValue(m_agentManager->getMasterAgent()->getBody()->getSkeletonNodeById(m_id)->getTranslation().y());
    m_SliderTz->setValue(m_agentManager->getMasterAgent()->getBody()->getSkeletonNodeById(m_id)->getTranslation().z());

    m_SliderRestTx->setValue(m_agentManager->getMasterAgent()->getBody()->getSkeletonNodeById(m_id)->getRestTranslation().x());
    m_SliderRestTy->setValue(m_agentManager->getMasterAgent()->getBody()->getSkeletonNodeById(m_id)->getRestTranslation().y());
    m_SliderRestTz->setValue(m_agentManager->getMasterAgent()->getBody()->getSkeletonNodeById(m_id)->getRestTranslation().z());
}

SegmentEditor::~SegmentEditor()
{
    delete ui;
}
