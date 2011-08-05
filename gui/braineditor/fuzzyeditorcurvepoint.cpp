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
