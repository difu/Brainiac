#include "latchcurveeditoritem.h"
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include "core/brainiacglobals.h"
#include "gui/Animation/loopeditorscene.h"


int LatchCurveEditorItem::type() const
{
    return Type;
}

LatchCurveEditorItem::LatchCurveEditorItem(LoopEditorScene *scene, LatchCurve *curve, int latchIndex) :
    QObject(),
    m_latchCurve(curve),
    m_latchIndex(latchIndex)
{
    scene->addItem(this);
    QCursor cursor;
    cursor.setShape(Qt::SizeHorCursor);
    setCursor(cursor);
    QColor col=BrainiacGlobals::DefaultLatchColor;
    QPen myPen(BrainiacGlobals::DefaultLatchColor);
    col.setAlpha(100);
    myPen.setColor(col);
    QBrush brush(col);
    brush.setStyle(Qt::SolidPattern);
    this->setBrush(brush);
    setRect();
}

LoopEditorScene* LatchCurveEditorItem::getLoopEditorScene() const
{
    LoopEditorScene *scene=qobject_cast<LoopEditorScene *>(this->scene());
    if(!scene) {
        qCritical() << __PRETTY_FUNCTION__ << "could not cast scene!";
    }
    return scene;
}

void LatchCurveEditorItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Figge";
}

void LatchCurveEditorItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Press " << event->button();
}

void LatchCurveEditorItem::setRect()
{
    LoopEditorScene *scene=getLoopEditorScene();
    LatchCurve::LatchType latch=m_latchCurve->latches().at(m_latchIndex);
    qreal height=scene->sceneRect().height();
    qreal startXPos=scene->mapTimeToWidth(latch.x());
    qreal xPosLength=scene->mapTimeToWidth(latch.y());
    QGraphicsRectItem::setRect(0.0,height/2.0,xPosLength,height/2.0);
    setPos(startXPos,0.0);
}
