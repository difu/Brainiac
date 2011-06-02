#include "outputeditor.h"
#include "ui_outputeditor.h"
#include "gui/brainiacslider.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/output.h"

#include <QDoubleValidator>

OutputEditor::OutputEditor(Scene *scene, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputEditor),
    ItemEditorWidgetsBase(scene)
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

void OutputEditor::manualResultChange(qreal value) {
    m_agentManager->setFuzzyResult(m_id, value);
    emit updateBrainEditor();
    emit updateGLContent();
    this->updateEditor();
}

void OutputEditor::setOutputConfig(AgentManager *manager,quint32 id)
{
    m_id=id;
    m_agentManager=manager;
    m_output=(Output *)m_agentManager->getMasterAgent()->getBrain()->getFuzzy(id);
    this->updateEditor();
}

void OutputEditor::updateEditor()
{
    ui->channelName->setText(m_output->getChannelName());
    ui->outputName->setText(m_output->getName());
    ui->rangeMax->setText(QString::number(m_output->getMaxValue()));
    ui->rangeMin->setText(QString::number(m_output->getMinValue()));
    m_slider->setValue(m_output->getResult());
}

OutputEditor::~OutputEditor()
{
    delete ui;
}

void OutputEditor::changeEvent(QEvent *e)
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
