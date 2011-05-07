#ifndef EDITORGRAPHICSVIEW_H
#define EDITORGRAPHICSVIEW_H

#include <QGraphicsView>

class EditorGraphicsView : public QGraphicsView
{
public:
    EditorGraphicsView();

protected:
    void keyPressEvent (QKeyEvent *keyEvent);
    void keyReleaseEvent(QKeyEvent *keyEvent);
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);
    bool m_rightButtonPressed;
    bool m_middleButtonPressed;
    int m_mousePosX;
    int m_mousePosY;
    int m_sceneCenterX;
    int m_sceneCenterY;
    QTransform *m_transformation;
};

#endif // EDITORGRAPHICSVIEW_H
