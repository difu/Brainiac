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


#include "editoritemconnector.h"
#include "editoritem.h"

#include <QtGui>
#include <math.h>
EditorItemConnector::EditorItemConnector(EditorItem *startItem, EditorItem *endItem,
         QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene)
{
    m_StartItem = startItem;
    m_EndItem = endItem;
    m_inverted = false;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    //setPen(QPen(m_Color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    m_StartItem->addConnector(this);
    m_EndItem->addConnector(this);
    setZValue(-1000.0);
}

QRectF EditorItemConnector::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

//QPainterPath EditorItemConnector::shape() const
//{
//    QPainterPath path = QGraphicsLineItem::shape();
//    //path.addPolygon(m_arrowHead);
//    return path;
//}

void EditorItemConnector::updatePosition()
{
    QLineF line(mapFromItem(m_StartItem, 0, 0), mapFromItem(m_EndItem, 0, 0));
    setLine(line);
}

void EditorItemConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    if (m_StartItem->collidesWithItem(m_EndItem))
        return;

    QLineF line(mapFromItem(m_StartItem, EditorItem::relxPos/2, EditorItem::relyPos/2), mapFromItem(m_EndItem, EditorItem::relxPos/2, EditorItem::relyPos/2));
    qreal angle = line.angle()/360*2*BrainiacGlobals::PI;
//    QLineF upperLine(mapFromItem(m_StartItem, 5*sin(angle), 5*cos(angle)), mapFromItem(m_EndItem, 0, 0));
//    QLineF lowerLine(mapFromItem(m_StartItem, -5*sin(angle), -5*cos(angle)), mapFromItem(m_EndItem, 0, 0));
    QPolygonF arrow;
    QPainterPath arrowPath;
    painter->drawLine(line);

    arrow << mapFromItem(m_StartItem, 5*sin(angle)+EditorItem::relxPos/2, 5*cos(angle)+EditorItem::relyPos/2) << mapFromItem(m_StartItem, -5*sin(angle)+EditorItem::relxPos/2, -5*cos(angle)+EditorItem::relyPos/2) << mapFromItem(m_EndItem, EditorItem::relxPos/2, EditorItem::relyPos/2)<< mapFromItem(m_StartItem, 5*sin(angle)+EditorItem::relxPos/2, 5*cos(angle)+EditorItem::relyPos/2);
    arrowPath.addPolygon(arrow);
    QColor arrowBorderColor;
    if( this->isSelected() ) {
        arrowBorderColor=Qt::red;
    } else {
        arrowBorderColor=QColor(79, 106, 25);
    }
    painter->setPen(QPen(arrowBorderColor, 1, Qt::SolidLine,
                     Qt::FlatCap, Qt::MiterJoin));
    if( m_inverted )
        painter->setBrush(QColor(20, 20, 39));
    else
        painter->setBrush(QColor(122, 163, 39));
    painter->drawPath(arrowPath);
    setLine(line);
    //qDebug() << "StartItem x:" << m_StartItem->pos().rx() << "StartItem y:" << m_StartItem->pos().ry() << "EndItem x:" << m_EndItem->pos().rx() << "EndItem y:" << m_EndItem->pos().ry();
}

