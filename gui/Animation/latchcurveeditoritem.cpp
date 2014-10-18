#include "latchcurveeditoritem.h"

int LatchCurveEditorItem::type() const
{
    return Type;
}

LatchCurveEditorItem::LatchCurveEditorItem(qreal x, qreal y, qreal width, qreal height) :
    QObject(),
    QGraphicsRectItem(x,y,width,height),
    m_yPos(.0)
{
}
