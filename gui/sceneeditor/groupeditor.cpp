// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "groupeditor.h"
#include "ui_groupeditor.h"
#include "core/group/group.h"

GroupEditor::GroupEditor(Scene *scene,QWidget *parent) :
    QWidget(parent),
    ItemEditorWidgetsBase(scene),
    ui(new Ui::GroupEditor)
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
