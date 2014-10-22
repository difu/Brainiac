#ifndef LATCHCURVEEDITORITEM_H
#define LATCHCURVEEDITORITEM_H

#include <QObject>
#include <QGraphicsRectItem>
#include "core/agent/body/animation/latchcurve.h"
class LoopEditorScene;
class LatchCurve;

class LatchCurveEditorItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 1 };

    int type() const;
    explicit LatchCurveEditorItem(LoopEditorScene *scene, LatchCurve *curve, int latchIndex);
    LoopEditorScene *getLoopEditorScene() const;
    void setRect();
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:

public slots:

private:
    LatchCurve *m_latchCurve;
    int m_latchIndex;

};

#endif // LATCHCURVEEDITORITEM_H
