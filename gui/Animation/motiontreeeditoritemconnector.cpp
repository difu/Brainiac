#include "motiontreeeditoritemconnector.h"
#include <QPainter>
MotionTreeEditorItemConnector::MotionTreeEditorItemConnector(EditorItem *startItem, EditorItem *endItem):EditorItemConnector(startItem, endItem)
{
    m_progress=0.0;
}

void MotionTreeEditorItemConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (m_StartItem->collidesWithItem(m_EndItem))
        return;

    static qreal _lineWidth=2.0;
    QLineF line(mapFromItem(m_StartItem, EditorItem::relxPos/2, EditorItem::relyPos/2), mapFromItem(m_EndItem, EditorItem::relxPos/2, EditorItem::relyPos/2));
    setLine(line);
    qreal angle = line.angle();
    angle+=20.0f;
    line.setAngle(angle);
    line.setLength(line.length()/2.0f);


    QPainterPath path;
    path.moveTo(mapFromItem(m_StartItem, EditorItem::relxPos/2, EditorItem::relyPos/2));
    path.cubicTo(line.p2(),line.p2(),mapFromItem(m_EndItem, EditorItem::relxPos/2, EditorItem::relyPos/2) );

    painter->setPen(QPen(QColor(79, 106, 25), _lineWidth, Qt::SolidLine,
                        Qt::FlatCap, Qt::MiterJoin));
    painter->setBrush(QBrush(Qt::transparent));

    painter->drawPath(path);

    if(m_progress>0.0 && m_progress <=1.0) {
        qreal _offset=0.0;
        qreal _progess=_offset+m_progress*(1.0-2*_offset);
        QPointF p;
        p=path.pointAtPercent(_progess);
        painter->setBrush(Qt::lightGray);

        painter->drawEllipse(p,10,10);
    }

}

MotionTreeEditorItemConnector::~MotionTreeEditorItemConnector()
{
}
