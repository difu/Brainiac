#include "fuzzyeditor.h"
#include "ui_fuzzyeditor.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/fuzzyfuzz.h"

FuzzyEditor::FuzzyEditor(Scene *scene, QWidget *parent) :
    QWidget(parent),
    ItemEditorWidgetsBase(scene),
    ui(new Ui::FuzzyEditor)
{
    ui->setupUi(this);
}

void FuzzyEditor::setFuzzConfig(AgentManager *manager, quint32 id)
{
    m_id=id;
    m_agentManager=manager;
    m_fuzz=(FuzzyFuzz *)m_agentManager->getMasterAgent()->getBrain()->getFuzzy(id);

    this->updateEditor();
}

void FuzzyEditor::updateEditor()
{
    ui->fuzzyName->setText(m_fuzz->getName());
}

FuzzyEditor::~FuzzyEditor()
{
    delete ui;
}

void FuzzyEditor::changeEvent(QEvent *e)
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
