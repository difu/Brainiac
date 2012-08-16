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


#include "fuzzyeditorcurveeditor.h"
#include <QKeyEvent>
#include "gui/braineditor/fuzzyeditorcurvepoint.h"
#include "core/agent/agentmanager.h"

FuzzyEditorCurveEditor::FuzzyEditorCurveEditor()
{
    m_fuzzCurve=new QGraphicsPathItem();
    addItem(m_fuzzCurve);
    m_curvePoint=new FuzzyEditorCurvePoint();
    addItem(m_curvePoint);
    connect(m_curvePoint,SIGNAL(pointMove(qreal)),this,SLOT(pointMoved(qreal)),Qt::DirectConnection);
    connect(m_curvePoint,SIGNAL(pointRelease()),this,SLOT(pointReleased()),Qt::DirectConnection);
}

void FuzzyEditorCurveEditor::setConfig(AgentManager *manager, FuzzyFuzz *fuzz)
{
    m_fuzz=fuzz;
    m_pointId=0;
    m_agentManager=manager;
    this->updateEditor();
}

qreal FuzzyEditorCurveEditor::mapYintoEditor(qreal yPos)
{
    return this->sceneRect().height()-3.0f-(yPos*this->sceneRect().height()*0.9f);
}

void FuzzyEditorCurveEditor::updateEditor()
{
    m_reducedWidth=this->sceneRect().width()-10;
    QPainterPath curvePath;
    curvePath.moveTo(0,0);
    for(qreal i=0; i<=this->sceneRect().width(); i+=1) {
        qreal value=m_fuzz->getFuzzOut(0,m_reducedWidth,i);
        curvePath.lineTo(i,mapYintoEditor(value));
        //qDebug() << i << mavalue*sceneRect.height();
    }
    m_fuzzCurve->setPen(QPen(Qt::green));
    m_fuzzCurve->setPath(curvePath);
    qreal upPos=3;
    qreal downPos=this->sceneRect().height()-8;
    qreal yPos=upPos;
    qreal lowerBound;
    qreal upperBound;
    switch(m_fuzz->getMode()) {
    case FuzzyFuzz::DIRAC:
        m_points=1;
        break;
    case FuzzyFuzz::ACTIVATE:
    case FuzzyFuzz::DEACTIVATE:
        m_points=2;
        break;
    case FuzzyFuzz::TRIANGLE:
        m_points=3;
        break;
    case FuzzyFuzz::TRAPEZOID:
        m_points=4;
        break;
    }
    qreal fuzzValue;
    if(m_pointId==0) {
        fuzzValue=m_fuzz->getP1();
        lowerBound=0;
        upperBound=m_fuzz->getP2();
        if(m_fuzz->getMode()==FuzzyFuzz::DEACTIVATE) {
            yPos=upPos;
        } else {
            yPos=downPos;
        }
    } else if(m_pointId==1) {
        fuzzValue=m_fuzz->getP2();
        lowerBound=m_fuzz->getP1();
        if(m_fuzz->getMode()==FuzzyFuzz::ACTIVATE || m_fuzz->getMode()==FuzzyFuzz::DEACTIVATE ) {
            upperBound=1;
        } else {
            upperBound=m_fuzz->getP3();
        }
        if(m_fuzz->getMode()==FuzzyFuzz::DEACTIVATE) {
            yPos=downPos;
        } else {
            yPos=upPos;
        }
    } else if(m_pointId==2) {
        fuzzValue=m_fuzz->getP3();
        lowerBound=m_fuzz->getP2();

        if(m_fuzz->getMode()==FuzzyFuzz::TRAPEZOID) {
            yPos=upPos;
            upperBound=m_fuzz->getP4();
        } else {
            yPos=downPos;
            upperBound=1;
        }
    } else if(m_pointId==3) {
        fuzzValue=m_fuzz->getP4();
        lowerBound=m_fuzz->getP3();
        upperBound=1;
        yPos=downPos;
    }
    //m_curvePoint->setPos(this->sceneRect().width()*fuzzValue,yPos);
    lowerBound*= m_reducedWidth;
    upperBound*= m_reducedWidth;
    m_curvePoint->setPosAndBoundsAbs(lowerBound,upperBound,m_reducedWidth*fuzzValue,yPos);
    //qDebug()<<"Pos" << this->sceneRect().width()*fuzzValue<<mapYintoEditor(yPos);
}

void FuzzyEditorCurveEditor::pointReleased()
{
    m_agentManager->setFuzzyFuzzMembershipPoints(m_fuzz->getId(), m_fuzz->getP1(),m_fuzz->getP2(), m_fuzz->getP3(),m_fuzz->getP4());
}

void FuzzyEditorCurveEditor::pointMoved(qreal xPos)
{
    qreal val=xPos/m_reducedWidth;
    if(m_pointId==0)
        m_fuzz->setP1(val,true);
    else if(m_pointId==1)
        m_fuzz->setP2(val,true);
    else if(m_pointId==2)
        m_fuzz->setP3(val,true);
    else if(m_pointId==3)
        m_fuzz->setP4(val,true);
    this->updateEditor();
    emit pointChange();
}

void FuzzyEditorCurveEditor::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Right) {
        m_pointId=(++m_pointId)%m_points;
    } else if(event->key()==Qt::Key_Left) {
        m_pointId=(--m_pointId)%m_points;
    }
    this->updateEditor();
}
