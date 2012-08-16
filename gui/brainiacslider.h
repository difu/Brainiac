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


#ifndef BRAINIACSLIDER_H
#define BRAINIACSLIDER_H

#include <QWidget>

#include "brainiacsliderframe.h"
class QLineEdit;

class BrainiacSlider : public QWidget
{
    Q_OBJECT
public:
    BrainiacSlider(QWidget *parent);
    void setRange(qreal param_min, qreal param_max);
    void setValue(qreal param_value);
    double getValue() {return m_value; }
    void setEnabled(bool param_enabled);
    void setGeometry(int x, int y, int w, int h);
    void setText(QString param_text);
    void setSliderColor(QColor param_color) {m_slider->setSliderColor(param_color);}
    void notifyReleaseButton(bool param_notify) {m_notify=param_notify;} //!< If true, emit changed only, if mousebutton is released.. Otherwise emit signal whenever slider is moved

protected:
    BrainiacSliderFrame *m_slider;
    QLineEdit *m_lineEdit;
    qreal m_min, m_max;
    qreal m_value;
    bool m_enabled;
    bool m_notify;
protected slots:
    void sliderValueChanged(qreal value);
    void sliderValueChangedAccepted(qreal value);
    void lineEditValueChanged(void);

signals:
    void valueChanged(qreal value);
};

#endif // BRAINIACSLIDER_H
