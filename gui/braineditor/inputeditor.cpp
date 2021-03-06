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


#include "inputeditor.h"
#include "gui/brainiacslider.h"
#include "core/agent/agentmanager.h"
#include "core/agent/brain/brainmanager.h"
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
    connect(m_slider,SIGNAL(valueChanged(qreal)),this,SLOT(manualResultChange(qreal)));
    connect(ui->inputName,SIGNAL(returnPressed()),this,SLOT(manualNameChange()));
    connect(ui->channelName,SIGNAL(returnPressed()),this,SLOT(manualChannelChange()));
    connect(ui->rangeMin,SIGNAL(returnPressed()),this,SLOT(manualMinMaxValueChange()));
    connect(ui->rangeMax,SIGNAL(returnPressed()),this,SLOT(manualMinMaxValueChange()));
}

InputEditor::~InputEditor()
{
    delete ui;
}

void InputEditor::manualChannelChange()
{
    m_agentManager->setFuzzyChannelName(m_id,ui->channelName->text());
    emit updateGLContent();
    this->updateEditor();
}

void InputEditor::manualMinMaxValueChange()
{
    m_agentManager->setFuzzyMinMax(m_id,ui->rangeMin->text().toDouble(),ui->rangeMax->text().toDouble());
    emit updateBrainEditor();
    emit updateGLContent();
    this->updateEditor();
}

void InputEditor::manualNameChange()
{
    m_agentManager->setFuzzyName(m_id,ui->inputName->text());
    emit updateBrainEditor();
    this->updateEditor();
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
    BrainManager *bm=m_agentManager->getBrainManager();
    ui->channelName->setText(bm->getFuzzyChannelName(m_input->getId()));
    ui->inputName->setText(bm->getFuzzyName(m_input->getId()));
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
