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


#include "fuzzyeditor.h"
#include "ui_fuzzyeditor.h"
#include "gui/braineditor/fuzzyeditorcurveeditor.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/fuzzyfuzz.h"
#include <QDebug>

FuzzyEditor::FuzzyEditor(Scene *scene, QWidget *parent) :
    QWidget(parent),
    ItemEditorWidgetsBase(scene),
    ui(new Ui::FuzzyEditor)
{
    ui->setupUi(this);
    ui->curveGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->curveGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_editor=new FuzzyEditorCurveEditor();
    m_editor->setSceneRect(0,0,ui->curveGraphicsView->geometry().width(),ui->curveGraphicsView->geometry().height());
    m_editor->setBackgroundBrush(QColor(10, 10, 10));
    ui->curveGraphicsView->setScene(m_editor);

    connect(ui->buttonLinearMode,SIGNAL(clicked()),this,SLOT(modeLinearPressed()));
    connect(ui->buttonQuadMode,SIGNAL(clicked()),this,SLOT(modeQuadPressed()));
    connect(ui->buttonSineMode,SIGNAL(clicked()),this,SLOT(modeSinePressed()));

    connect(ui->buttonActivate,SIGNAL(clicked()), this, SLOT(modeActivatePressed()));
    connect(ui->buttonDeactivate,SIGNAL(clicked()), this, SLOT(modeDeactivatePressed()));
    connect(ui->buttonTriangle,SIGNAL(clicked()), this, SLOT(modeTrianglePressed()));
    connect(ui->buttonTrapezoid,SIGNAL(clicked()), this, SLOT(modeTrapezoidPressed()));
    connect(ui->buttonDirac,SIGNAL(clicked()), this, SLOT(modeDiracPressed()));

    connect(ui->fuzzyName,SIGNAL(returnPressed()),this,SLOT(manualNameChanged()));

    connect(m_editor,SIGNAL(pointChange()),this,SLOT(pointChanged()),Qt::DirectConnection);
}

void FuzzyEditor::manualNameChanged()
{
    m_agentManager->setFuzzyName(m_id,ui->fuzzyName->text());
    emit updateBrainEditor();
    updateEditor();
}

void FuzzyEditor::modeLinearPressed()
{
    m_agentManager->setFuzzyFuzzInterpolationMode(m_id,FuzzyFuzz::LINEAR);
    updateEditor();
}

void FuzzyEditor::modeSinePressed()
{
    m_agentManager->setFuzzyFuzzInterpolationMode(m_id,FuzzyFuzz::SINE);
    updateEditor();
}

void FuzzyEditor::modeQuadPressed()
{
    m_agentManager->setFuzzyFuzzInterpolationMode(m_id,FuzzyFuzz::QUAD);
    updateEditor();
}

void FuzzyEditor::modeDiracPressed()
{
    m_agentManager->setFuzzyFuzzMode(m_id, FuzzyFuzz::DIRAC);
    updateEditor();
}

void FuzzyEditor::modeActivatePressed()
{
    m_agentManager->setFuzzyFuzzMode(m_id, FuzzyFuzz::ACTIVATE);
    updateEditor();
}

void FuzzyEditor::modeDeactivatePressed()
{
    m_agentManager->setFuzzyFuzzMode(m_id, FuzzyFuzz::DEACTIVATE);
    updateEditor();
}

void FuzzyEditor::modeTrianglePressed()
{
    m_agentManager->setFuzzyFuzzMode(m_id, FuzzyFuzz::TRIANGLE);
    updateEditor();
}

void FuzzyEditor::modeTrapezoidPressed()
{
    m_agentManager->setFuzzyFuzzMode(m_id, FuzzyFuzz::TRAPEZOID);
    updateEditor();
}

void FuzzyEditor::pointChanged()
{
    quint32 id=m_editor->getPointId();
    qreal val=0;
    if(id==0)
        val=m_fuzz->getP1Abs();
    else if(id==1)
        val=m_fuzz->getP2Abs();
    else if(id==2)
        val=m_fuzz->getP3Abs();
    else if(id==3)
        val=m_fuzz->getP4Abs();
    ui->value_lineEdit->setText(QString::number(val));
}

void FuzzyEditor::setFuzzConfig(AgentManager *manager, quint32 id)
{
    m_id=id;
    m_agentManager=manager;
    m_fuzz=(FuzzyFuzz *)m_agentManager->getMasterAgent()->getBrain()->getFuzzy(id);
    m_editor->setConfig(m_agentManager, m_fuzz);
    this->updateEditor();
}

void FuzzyEditor::updateEditor()
{
    m_editor->updateEditor();
    ui->fuzzyName->setText(m_fuzz->getName());
    ui->buttonLinearMode->setChecked(m_fuzz->getInterpolationMode()==FuzzyFuzz::LINEAR);
    ui->buttonQuadMode->setChecked(m_fuzz->getInterpolationMode()==FuzzyFuzz::QUAD);
    ui->buttonSineMode->setChecked(m_fuzz->getInterpolationMode()==FuzzyFuzz::SINE);

    ui->buttonDirac->setChecked(m_fuzz->getMode()==FuzzyFuzz::DIRAC);
    ui->buttonActivate->setChecked(m_fuzz->getMode()==FuzzyFuzz::ACTIVATE);
    ui->buttonDeactivate->setChecked(m_fuzz->getMode()==FuzzyFuzz::DEACTIVATE);
    ui->buttonTriangle->setChecked(m_fuzz->getMode()==FuzzyFuzz::TRIANGLE);
    ui->buttonTrapezoid->setChecked(m_fuzz->getMode()==FuzzyFuzz::TRAPEZOID);
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
