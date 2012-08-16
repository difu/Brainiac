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


#include "fuzzyeditorcurvepoint.h"
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

FuzzyEditorCurvePoint::FuzzyEditorCurvePoint() :
    QObject(), QGraphicsRectItem()
{
    setRect(0,0,5,5);
    setPen(QPen(Qt::red));
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void FuzzyEditorCurvePoint::setPosAndBoundsAbs(qreal min, qreal max, qreal posX, qreal posY)
{
    m_maxX=max;
    m_minX=min;
    m_yPos=posY;
    this->setPos(posX,posY);
}

void FuzzyEditorCurvePoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    qreal xPos=qBound(m_minX,event->scenePos().x(),m_maxX);
    this->setPos(xPos,m_yPos);
    emit pointMove(xPos);
}

void FuzzyEditorCurvePoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    emit pointRelease();
}
