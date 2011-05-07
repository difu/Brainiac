#include "editorgraphicsview.h"

EditorGraphicsView::EditorGraphicsView():QGraphicsView()
{
    setDragMode(QGraphicsView::RubberBandDrag);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //setDragMode(QGraphicsView::ScrollHandDrag);
}

void EditorGraphicsView::keyPressEvent(QKeyEvent *keyEvent)
{
    QGraphicsView::keyPressEvent(keyEvent);
}

void EditorGraphicsView::keyReleaseEvent(QKeyEvent *keyEvent)
{
    QGraphicsView::keyReleaseEvent(keyEvent);
}

void EditorGraphicsView::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    QGraphicsView::mouseMoveEvent(mouseEvent);
}

void EditorGraphicsView::mousePressEvent(QMouseEvent *mouseEvent)
{
    QGraphicsView::mousePressEvent(mouseEvent);
}

void EditorGraphicsView::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    QGraphicsView::mouseReleaseEvent(mouseEvent);
}
