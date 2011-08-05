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
