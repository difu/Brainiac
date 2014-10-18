#ifndef LATCHCURVEEDITORITEM_H
#define LATCHCURVEEDITORITEM_H

#include <QObject>
#include <QGraphicsRectItem>

class LatchCurveEditorItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 1 };

    int type() const;
    explicit LatchCurveEditorItem(qreal x, qreal y, qreal width, qreal height);

signals:

public slots:

private:
    qreal m_yPos;

};

#endif // LATCHCURVEEDITORITEM_H
