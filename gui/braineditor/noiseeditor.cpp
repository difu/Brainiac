#include "noiseeditor.h"
#include "ui_noiseeditor.h"
#include "gui/brainiacslider.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/noise.h"

NoiseEditor::NoiseEditor(Scene *scene, QWidget *parent) :
    QWidget(parent),
    ItemEditorWidgetsBase(scene),
    ui(new Ui::NoiseEditor)
{
    ui->setupUi(this);
    m_agentManager=0;
    m_sliderRate = new BrainiacSlider(this);
    m_sliderRate->setGeometry(10,100,400,40);
    m_sliderResult = new BrainiacSlider(this);
    m_sliderResult->setGeometry(10,60,400,40);
    connect(ui->noiseName,SIGNAL(returnPressed()),this,SLOT(manualNameChange()));
    connect(m_sliderResult,SIGNAL(valueChanged(qreal)),this,SLOT(manualResultChange(qreal)));
    connect(m_sliderRate,SIGNAL(valueChanged(qreal)),this,SLOT(manualRateChange(qreal)));
}

NoiseEditor::~NoiseEditor()
{
    delete ui;
}

void NoiseEditor::manualNameChange()
{
    m_agentManager->setFuzzyName(m_id,ui->noiseName->text());
    emit updateBrainEditor();
}

void NoiseEditor::manualRateChange(qreal value)
{
    m_agentManager->setNoiseRate(m_id,value);
}

void NoiseEditor::manualResultChange(qreal value)
{
    m_agentManager->setFuzzyResult(m_id,value);
    emit updateBrainEditor();
}

void NoiseEditor::setNoiseConfig(AgentManager *manager, quint32 id)
{
    m_agentManager=manager;
    m_id=id;
    m_noise=(Noise *)m_agentManager->getMasterAgent()->getBrain()->getFuzzy(id);
    Q_ASSERT(m_noise->getType()==FuzzyBase::NOISE);
    this->updateEditor();
}

void NoiseEditor::updateEditor()
{
    ui->noiseName->setText(m_noise->getName());
    m_sliderRate->setRange(0,m_noise->getMaxRate());
    m_sliderRate->setValue(m_noise->getRate());
    m_sliderResult->setValue(m_noise->getResult());
    m_sliderResult->setRange(m_noise->getMinValue(),m_noise->getMaxValue());

}

void NoiseEditor::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
