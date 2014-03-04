#include "motiontreeeditoritemconnector.h"
#include <QPainter>
MotionTreeEditorItemConnector::MotionTreeEditorItemConnector(EditorItem *startItem, EditorItem *endItem):EditorItemConnector(startItem, endItem)
{
    m_progress=0.0;
}

void MotionTreeEditorItemConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (m_StartItem->collidesWithItem(m_EndItem))
        return;

    painter->setBrush(QColor(122, 163, 39));
    QLineF line(mapFromItem(m_StartItem, EditorItem::relxPos/2, EditorItem::relyPos/2), mapFromItem(m_EndItem, EditorItem::relxPos/2, EditorItem::relyPos/2));
    setLine(line);
    qreal angle = line.angle();
    angle+=20.0f;
    line.setAngle(angle);
    line.setLength(line.length()/2.0f);
    painter->drawLine(line);
    QLineF line2(line.p2(), mapFromItem(m_EndItem, EditorItem::relxPos/2, EditorItem::relyPos/2));
    painter->drawLine(line2);
    if(m_progress>0.0 && m_progress <=1.0) {
        QPointF p;
        if(m_progress<0.5) {
            p=line.pointAt(m_progress*2.0);
        } else {
            p=line2.pointAt((m_progress-0.5)*2.0);
        }
        painter->drawEllipse(p,10,10);
    }

}

MotionTreeEditorItemConnector::~MotionTreeEditorItemConnector()
{
}
