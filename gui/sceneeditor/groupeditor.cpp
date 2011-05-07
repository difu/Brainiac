#include "groupeditor.h"
#include "ui_groupeditor.h"
#include "core/group/group.h"

GroupEditor::GroupEditor(Scene *scene,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupEditor),
    ItemEditorWidgetsBase(scene)
{
    ui->setupUi(this);
    connect(ui->groupName,SIGNAL(returnPressed()),this,SLOT(groupNameChanged()));
}

void GroupEditor::groupNameChanged()
{
    m_group->setName(ui->groupName->text());
}

void GroupEditor::setGroup( Group* group )
{
    m_group=group;
    ui->groupName->setText(m_group->getName());
    ui->agentFile->setText(m_group->getAgentFileName());
}

GroupEditor::~GroupEditor()
{
    delete ui;
}

void GroupEditor::changeEvent(QEvent *e)
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
