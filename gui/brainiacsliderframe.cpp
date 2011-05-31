#include "brainiacsliderframe.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

BrainiacSliderFrame::BrainiacSliderFrame(QWidget *parent) : QFrame(parent)
{
    m_pressedXPos=0;
    m_mousePressed=false;
    m_zeroMark=0;
    m_sliderColor=Qt::lightGray;
    m_text="";
}

void BrainiacSliderFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    QPainter painter(this);
    painter.setBrush(m_sliderColor);
    painter.drawRect(1,1,m_pressedXPos,28);
    painter.setBrush(Qt::white);
    painter.drawLine(this->width()*m_zeroMark,1,this->width()*m_zeroMark,29);
    //qDebug() << "Zeromarker at"<< m_zeroMark;
    painter.setFont(QFont("Arial", 25));
    painter.setPen(Qt::cyan);
    painter.drawText(rect(), Qt::AlignCenter, m_text);
}

void BrainiacSliderFrame::setText(QString text)
{
    m_text=text;
}

void BrainiacSliderFrame::setVolume(qreal param_value)
{
    if(param_value<0)
        param_value=0;
    else if(param_value>1)
        param_value=1;
    m_pressedXPos=this->width()*param_value;
    m_value=param_value;
    this->update();

}

void BrainiacSliderFrame::mousePressEvent(QMouseEvent *event)
{
    m_mousePressed=true;
    m_pressedXPos=event->x();
    m_value=(double)event->x()/(double)this->width();
    this->update();
    emit valueChanged(m_value);
}

void BrainiacSliderFrame::mouseMoveEvent(QMouseEvent *event)
{
    m_pressedXPos=event->x();
    m_value=(double)event->x()/(double)this->width();
    //qDebug()<< "Slider:" << m_value << m_enabled;
    this->update();
    emit valueChanged(m_value);
}

void BrainiacSliderFrame::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_mousePressed=false;
    emit valueChangedAccepted(m_value);
}
