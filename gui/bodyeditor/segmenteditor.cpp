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


#include "segmenteditor.h"
#include "gui/brainiacslider.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/bodymanager.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include "core/agent/body/bodysegment.h"
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
    createDimensionSliders();
    m_editSymetric=true;
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

void SegmentEditor::createDimensionSliders()
{
    m_SliderDimensionX=new BrainiacSlider(ui->frameShapeDims);
    m_SliderDimensionY=new BrainiacSlider(ui->frameShapeDims);
    m_SliderDimensionZ=new BrainiacSlider(ui->frameShapeDims);

    m_SliderSphereRadius=new BrainiacSlider(ui->frameShapeDims);

    m_SliderTubeDiameter=new BrainiacSlider(ui->frameShapeDims);
    m_SliderTubeLength=new BrainiacSlider(ui->frameShapeDims);

    m_SliderDimensionX->setSliderColor(BrainiacGlobals::defaultXColor);
    m_SliderDimensionY->setSliderColor(BrainiacGlobals::defaultYColor);
    m_SliderDimensionZ->setSliderColor(BrainiacGlobals::defaultZColor);

    m_SliderSphereRadius->setSliderColor(BrainiacGlobals::defaultRadiusColor);

    m_SliderTubeDiameter->setSliderColor(BrainiacGlobals::defaultDiameterColor);
    m_SliderTubeLength->setSliderColor(BrainiacGlobals::defaultLengthColor);

    m_SliderDimensionX->setGeometry(0,2,300,40);
    m_SliderDimensionY->setGeometry(0,35,300,40);
    m_SliderDimensionZ->setGeometry(0,69,300,40);

    m_SliderSphereRadius->setGeometry(0,2,300,40);

    m_SliderTubeDiameter->setGeometry(0,35,300,40);
    m_SliderTubeLength->setGeometry(0,69,300,40);

    m_SliderDimensionX->setText("x");
    m_SliderDimensionY->setText("y");
    m_SliderDimensionZ->setText("z");

    m_SliderSphereRadius->setText("radius");

    m_SliderTubeDiameter->setText("diameter");
    m_SliderTubeLength->setText("length");

    m_SliderDimensionX->setRange(0.0f,50.0f);
    m_SliderDimensionY->setRange(0.0f,50.0f);
    m_SliderDimensionZ->setRange(0.0f,50.0f);

    m_SliderSphereRadius->setRange(0.0f,50.0f);
    m_SliderTubeDiameter->setRange(0.0f,50.0f);
    m_SliderTubeLength->setRange(0.0f,50.0f);

    connect(m_SliderDimensionX,SIGNAL(valueChanged(qreal)),this,SLOT(manualDimensionsChanged(qreal)));
    connect(m_SliderDimensionY,SIGNAL(valueChanged(qreal)),this,SLOT(manualDimensionsChanged(qreal)));
    connect(m_SliderDimensionZ,SIGNAL(valueChanged(qreal)),this,SLOT(manualDimensionsChanged(qreal)));
    connect(m_SliderSphereRadius,SIGNAL(valueChanged(qreal)),this,SLOT(manualDimensionsChanged(qreal)));
    connect(m_SliderTubeDiameter,SIGNAL(valueChanged(qreal)),this,SLOT(manualDimensionsChanged(qreal)));
    connect(m_SliderTubeLength,SIGNAL(valueChanged(qreal)),this,SLOT(manualDimensionsChanged(qreal)));
}

void SegmentEditor::editSymetric(bool editSymetric)
{
    m_editSymetric=editSymetric;
}

quint32 SegmentEditor::getSymetricSegmentId() const
{
    Segment *seg=m_agentManager->getBodyManager()->getSegments().value(m_id);
    if(seg) {
        QString name=seg->getName();
        QString otherName;
        if(name.startsWith('r')) {
            otherName="l";
        } else if(name.startsWith('l')) {
            otherName="r";
        } else {
            return 0;
        }
        otherName=otherName%name.mid(1);

        foreach(Segment *otherSeg, m_agentManager->getBodyManager()->getSegments()) {
            if(QString::compare(name,otherName,Qt::CaseSensitive)==0) {
                return otherSeg->getId();
            }
        }
    } else {
        qWarning() << __PRETTY_FUNCTION__ << "No segment with id "<< m_id << "found!";
        return 0;
    }
    return 0;
}

void SegmentEditor::manualDimensionsChanged(qreal value) {
    Q_UNUSED(value);
    switch(m_agentManager->getBodyManager()->getSegment(m_id).getType()) {
    case BrainiacGlobals::BOXSEGMENT:
        m_agentManager->setSegmentDimensions(m_id,m_SliderDimensionX->getValue(),m_SliderDimensionY->getValue(),m_SliderDimensionZ->getValue());
        if(m_editSymetric) {
            quint32 otherId=getSymetricSegmentId();
            //m_agentManager->setSegmentDimensions(otherId,m_SliderDimensionX->getValue(),m_SliderDimensionY->getValue(),m_SliderDimensionZ->getValue());
        }
        break;
    case BrainiacGlobals::SPHERESEGMENT:
        m_agentManager->setSegmentDimensions(m_id,m_SliderSphereRadius->getValue(),m_SliderSphereRadius->getValue(),m_SliderSphereRadius->getValue());
        if(m_editSymetric) {
            quint32 otherId=getSymetricSegmentId();
            //m_agentManager->setSegmentDimensions(otherId,m_SliderSphereRadius->getValue(),m_SliderSphereRadius->getValue(),m_SliderSphereRadius->getValue());
        }
        break;
    case BrainiacGlobals::TUBESEGMENT:
        m_agentManager->setSegmentDimensions(m_id,m_SliderTubeDiameter->getValue(),m_SliderTubeDiameter->getValue(),m_SliderTubeLength->getValue());
        if(m_editSymetric) {
            quint32 otherId=getSymetricSegmentId();
            //m_agentManager->setSegmentDimensions(otherId,m_SliderTubeDiameter->getValue(),m_SliderTubeLength->getValue(),m_SliderTubeDiameter->getValue());
        }
        break;
    default:
        qCritical() << __PRETTY_FUNCTION__ << "Segment type" << m_agentManager->getBodyManager()->getSegment(m_id).getType() << "not yet implemented!";
        break;
    }


    emit updateGLContent();
    this->updateEditor();
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
    if(m_editSymetric) {
        quint32 otherId=getSymetricSegmentId();
        m_agentManager->setSegmentRotation(otherId,m_SliderRx->getValue(),m_SliderRy->getValue(),m_SliderRz->getValue());
    }

    emit updateGLContent();
    this->updateEditor();
}

void SegmentEditor::manualTransChanged(qreal value)
{
    Q_UNUSED(value);
    m_agentManager->setSegmentTranslation(m_id,m_SliderTx->getValue(),m_SliderTy->getValue(),m_SliderTz->getValue());
    if(m_editSymetric) {
        quint32 otherId=getSymetricSegmentId();
        m_agentManager->setSegmentTranslation(otherId,m_SliderTx->getValue(),m_SliderTy->getValue(),-m_SliderTz->getValue());
    }
    emit updateGLContent();
    this->updateEditor();
}

void SegmentEditor::setSegmentConfig(AgentManager *manager, quint32 id)
{
    m_agentManager=manager;
    m_id=id;

    Segment s=m_agentManager->getBodyManager()->getSegment(m_id);
    m_SliderDimensionX->setVisible(s.getType()==BrainiacGlobals::BOXSEGMENT);
    m_SliderDimensionY->setVisible(s.getType()==BrainiacGlobals::BOXSEGMENT);
    m_SliderDimensionZ->setVisible(s.getType()==BrainiacGlobals::BOXSEGMENT);

    m_SliderSphereRadius->setVisible(s.getType()==BrainiacGlobals::SPHERESEGMENT);

    m_SliderTubeDiameter->setVisible(s.getType()==BrainiacGlobals::TUBESEGMENT);
    m_SliderTubeLength->setVisible(s.getType()==BrainiacGlobals::TUBESEGMENT);
    m_agentManager->getBodyAgent()->getBody()->highlightSegment(m_id);
    updateEditor();
}

void SegmentEditor::updateEditor()
{
    QVector3D rot=m_agentManager->getBodyManager()->getSegmentRotation(m_id);
    m_SliderRx->setValue(rot.x());
    m_SliderRy->setValue(rot.y());
    m_SliderRz->setValue(rot.z());

    QVector3D restRot=m_agentManager->getBodyManager()->getSegmentRestRotation(m_id);
    m_SliderRestRx->setValue(restRot.x());
    m_SliderRestRy->setValue(restRot.y());
    m_SliderRestRz->setValue(restRot.z());

    QVector3D trans=m_agentManager->getBodyManager()->getSegmentTranslation(m_id);
    m_SliderTx->setValue(trans.x());
    m_SliderTy->setValue(trans.y());
    m_SliderTz->setValue(trans.z());

    QVector3D restTrans=m_agentManager->getBodyManager()->getSegmentRestTranslation(m_id);
    m_SliderRestTx->setValue(restTrans.x());
    m_SliderRestTy->setValue(restTrans.y());
    m_SliderRestTz->setValue(restTrans.z());

    QVector3D scale=m_agentManager->getBodyManager()->getSegmentScale(m_id);

    switch(m_agentManager->getBodyManager()->getSegment(m_id).getType()) {
    case BrainiacGlobals::BOXSEGMENT:
        m_SliderDimensionX->setValue(scale.x());
        m_SliderDimensionY->setValue(scale.y());
        m_SliderDimensionZ->setValue(scale.z());
        break;
    case BrainiacGlobals::SPHERESEGMENT:
        m_SliderSphereRadius->setValue(scale.x());
        break;
    case BrainiacGlobals::TUBESEGMENT:
        m_SliderTubeDiameter->setValue(scale.z());
        m_SliderTubeLength->setValue(scale.x());
        break;
    default:
        qCritical() << __PRETTY_FUNCTION__ << "Segment type" << m_agentManager->getBodyManager()->getSegment(m_id).getType() << "not yet implemented!";
        break;
    }


}

SegmentEditor::~SegmentEditor()
{
    delete ui;
}
