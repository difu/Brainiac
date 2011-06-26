#include "inputeditor.h"
#include "gui/brainiacslider.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/input.h"
#include "ui_inputeditor.h"

InputEditor::InputEditor(Scene *scene, QWidget *parent) :
    QWidget(parent),
    ItemEditorWidgetsBase(scene),
    ui(new Ui::InputEditor)
{
    ui->setupUi(this);
    m_agentManager=0;
    m_slider = new BrainiacSlider(this);
    m_slider->setGeometry(10,60,400,40);
    ui->rangeMax->setValidator(new QDoubleValidator(ui->rangeMax));
    ui->rangeMin->setValidator(new QDoubleValidator(ui->rangeMin));
//    m_ui->lineEdit_rangeStop->setValidator(new QDoubleValidator(m_ui->lineEdit_rangeStop));
//    m_ui->lineEdit_rangeStart->setValidator(new QDoubleValidator(m_ui->lineEdit_rangeStart));
    connect(m_slider,SIGNAL(valueChanged(qreal)),this,SLOT(manualResultChange(qreal)));
}

InputEditor::~InputEditor()
{
    delete ui;
}

void InputEditor::manualResultChange(qreal value)
{
    m_agentManager->setFuzzyResult(m_id, value);
    emit updateBrainEditor();
    emit updateGLContent();
    this->updateEditor();
}

void InputEditor::setInputConfig(AgentManager *manager, quint32 id)
{
    m_id=id;
    m_agentManager=manager;
    m_input=(Input *)m_agentManager->getMasterAgent()->getBrain()->getFuzzy(id);
    this->updateEditor();
}

void InputEditor::updateEditor()
{
    ui->channelName->setText(m_input->getChannelName());
    ui->outputName->setText(m_input->getName());
    ui->rangeMax->setText(QString::number(m_input->getMaxValue()));
    ui->rangeMin->setText(QString::number(m_input->getMinValue()));
    m_slider->setRange(m_input->getMinValue(),m_input->getMaxValue());
    m_slider->setValue(m_input->getResult());
}

void InputEditor::changeEvent(QEvent *e)
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
