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


#ifndef BRAINIACSLIDERFRAME_H
#define BRAINIACSLIDERFRAME_H

#include <QFrame>

class BrainiacSliderFrame : public QFrame
{
    Q_OBJECT
public:
    BrainiacSliderFrame(QWidget *parent);

    void setVolume(qreal param_value);
    void markZero(qreal param_zero) {m_zeroMark=param_zero;}
    void setSliderColor(QColor param_col) {m_sliderColor=param_col;}
    void setText(QString text);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    int m_pressedXPos;
    qreal m_value;
    bool m_mousePressed;
    float m_zeroMark;
    QColor m_sliderColor;
    QString m_text;
signals:
    void valueChanged(qreal value);
    void valueChangedAccepted(qreal value);
};

#endif // BRAINIACSLIDERFRAME_H
