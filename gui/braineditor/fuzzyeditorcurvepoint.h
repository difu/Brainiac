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


#ifndef FUZZYEDITORCURVEPOINT_H
#define FUZZYEDITORCURVEPOINT_H

#include <QObject>
#include <QGraphicsRectItem>

/** \brief  A rectangle representing a membership point in the FuzzyEditorCurveEditor

        @sa FuzzyEditorCurveEditor
**/
class FuzzyEditorCurvePoint : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    /** \brief  Constructs a new FuzzyEditorCurvePoint
    **/
    explicit FuzzyEditorCurvePoint();
    void setPosAndBoundsAbs(qreal min, qreal max, qreal posX, qreal posY);

protected:
    qreal m_yPos;
    qreal m_minX;
    qreal m_maxX;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:
    void pointMove(qreal pos);
    void pointRelease();

public slots:

};

#endif // FUZZYEDITORCURVEPOINT_H
