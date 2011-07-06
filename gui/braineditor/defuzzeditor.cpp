#include "defuzzeditor.h"
#include "ui_defuzzeditor.h"
#include "core/agent/brain/fuzzydefuzz.h"
#include "core/agent/brain/output.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/brain/brain.h"
#include "gui/brainiacslider.h"

DefuzzEditor::DefuzzEditor(Scene *scene, QWidget *parent) :
    QWidget(parent),
    ItemEditorWidgetsBase(scene),
    ui(new Ui::DefuzzEditor)
{
    ui->setupUi(this);
    m_agentManager=0;
    m_fuzzValSlider=new BrainiacSlider(this);
    m_fuzzValSlider->setGeometry(10,60,400,40);
    m_fuzzResultSlider=new BrainiacSlider(this);
    m_fuzzResultSlider->setGeometry(10,100,400,40);
    m_fuzzResultSlider->setEnabled(false);
    m_fuzzValSlider->notifyReleaseButton(true);
    connect(m_fuzzValSlider,SIGNAL(valueChanged(qreal)),this,SLOT(manualDefuzzValueChange(qreal)));
    connect(ui->defuzzName,SIGNAL(returnPressed()),this,SLOT(manualNameChange()));
    connect(ui->isElse_pb,SIGNAL(toggled(bool)),this,SLOT(manualIsElseChange(bool)));
}

void DefuzzEditor::manualDefuzzValueChange(qreal value)
{
    m_agentManager->setDefuzzValue(m_id, value);
    emit updateBrainEditor();
    emit updateGLContent();
    this->updateEditor();
}

void DefuzzEditor::manualNameChange()
{
    m_agentManager->setFuzzyName(m_id,ui->defuzzName->text());
    emit updateBrainEditor();
    this->updateEditor();
}

void DefuzzEditor::manualIsElseChange(bool isChecked)
{
    if(isChecked) {
        m_agentManager->setDefuzzIsElse(m_id,true);
    } else {
        m_agentManager->setDefuzzIsElse(m_id,false);
    }
}

void DefuzzEditor::setDefuzzConfig(AgentManager *manager, quint32 id)
{
    m_id=id;
    m_agentManager=manager;
    m_defuzz=(FuzzyDefuzz *)m_agentManager->getMasterAgent()->getBrain()->getFuzzy(id);


    this->updateEditor();
}

void DefuzzEditor::updateEditor()
{
    ui->defuzzName->setText(m_defuzz->getName());
    if(m_defuzz->hasChildren()) {
        FuzzyBase *base=m_defuzz->getChildren().at(0);
        if(base->getType()==FuzzyBase::OUTPUT) {
            Output *output=(Output *) base;
            m_fuzzValSlider->setRange(output->getMinValue(),output->getMaxValue());
            m_fuzzValSlider->setValue(BrainiacGlobals::deNorm(output->getMinValue(),output->getMaxValue(),m_defuzz->getDefuzzVal()));
        } else {
            m_fuzzValSlider->setRange(0,1);
            m_fuzzValSlider->setValue(0);
        }
        m_fuzzResultSlider->setRange(m_defuzz->getMinValue(),m_defuzz->getMaxValue());
        m_fuzzResultSlider->setValue(m_defuzz->getResult());
    }
    if(m_defuzz->isElse())
        ui->isElse_pb->setChecked(true);
    else
        ui->isElse_pb->setChecked(false);

}

DefuzzEditor::~DefuzzEditor()
{
    delete ui;
}

void DefuzzEditor::changeEvent(QEvent *e)
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
