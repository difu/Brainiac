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


#include "brainiacslider.h"
#include "brainiacsliderframe.h"
#include <QFrame>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QDebug>


BrainiacSlider::BrainiacSlider(QWidget *parent):QWidget(parent)
{
    QWidget::setGeometry(0,0,350,40);
    m_slider=new BrainiacSliderFrame(this);
    m_lineEdit=new QLineEdit(this);
    m_lineEdit->setValidator(new QDoubleValidator(m_lineEdit));
    m_slider->setGeometry(0,0,250,30);
    m_slider->setFrameStyle(QFrame::Panel);
    m_lineEdit->setGeometry(255,0,100,30);
    m_max=1;m_min=0;m_value=0;
    m_enabled=true;
    m_slider->setVolume(0.0);
    m_notify=true;
    connect(m_lineEdit,SIGNAL(returnPressed()),this,SLOT(lineEditValueChanged()));
    connect(m_slider,SIGNAL(valueChanged(qreal)),this,SLOT(sliderValueChanged(qreal)));
    connect(m_slider,SIGNAL(valueChangedAccepted(qreal)),this,SLOT(sliderValueChangedAccepted(qreal)));
}

void BrainiacSlider::setText(QString param_text)
{
    m_slider->setText(param_text);
}

void BrainiacSlider::setEnabled(bool param_enabled)
{
    m_enabled=param_enabled;
    m_lineEdit->setEnabled(param_enabled);
    m_slider->setEnabled(param_enabled);
}

void BrainiacSlider::setGeometry(int x, int y, int w, int h) {
    QWidget::setGeometry(x,y,w,h);
    quint32 newW=(quint32)(0.5f*(qreal)w);
    //quint32 newW=(quint32)(0,7f*(qreal)w);
    m_slider->setGeometry(0,0,newW,h-10);
    m_lineEdit->setGeometry(newW,0,100,30);
}

void BrainiacSlider::setRange(qreal param_min, qreal param_max)
{
    if(param_max>param_min){
        m_max=param_max;
        m_min=param_min;
    } else {
        m_max=param_min;
        m_min=param_max;
    }
    m_slider->markZero((0-m_min)/(m_max-m_min));
}

void BrainiacSlider::setValue(qreal param_value)
{
    if(param_value<m_min)
        param_value=m_min;
    else if(param_value>m_max)
        param_value=m_max;
    m_value=param_value;
    m_lineEdit->setText(QString::number(m_value));
    m_slider->setVolume((m_value-m_min)/(m_max-m_min));
}

void BrainiacSlider::sliderValueChanged(qreal value)
{
    qreal _val=value*(m_max-m_min)+m_min;
    if(_val<m_min)
        _val=m_min;
    else if(_val>m_max)
        _val=m_max;
    this->setValue(_val);
    if(m_notify) {
        emit valueChanged(_val);
    }
}

void BrainiacSlider::sliderValueChangedAccepted(qreal value)
{
    qreal _val=value*(m_max-m_min)+m_min;
    if(_val<m_min)
        _val=m_min;
    else if(_val>m_max)
        _val=m_max;
    this->setValue(_val);
    emit valueChanged(_val);
}

void BrainiacSlider::lineEditValueChanged()
{
    this->setValue(m_lineEdit->text().toDouble());
    emit valueChanged(m_lineEdit->text().toDouble());
}
