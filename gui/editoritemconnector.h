#ifndef EDITORITEMCONNECTOR_H
#define EDITORITEMCONNECTOR_H

#include <QGraphicsLineItem>

class EditorItem;


/** \brief  graphical connector on a QGraphicsScene


        Class for displaying a connector between two editor items on a QGraphicsScene



**/

class EditorItemConnector : public QGraphicsLineItem
{

public:
    enum { Type = UserType + 4 };

    EditorItemConnector(EditorItem *startItem, EditorItem *endItem,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
        { return Type; }
    QRectF boundingRect() const;
//    QPainterPath shape() const;
    EditorItem *startItem() const
        { return m_StartItem; }
    EditorItem *endItem() const
        { return m_EndItem; }
    void setInverted( bool inverted )
        { m_inverted = inverted; }
    bool isInverted() {return m_inverted; }


public slots:
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
    bool m_inverted;
    EditorItem *m_StartItem;
    EditorItem *m_EndItem;
};
#endif // EDITORITEMCONNECTOR_H
