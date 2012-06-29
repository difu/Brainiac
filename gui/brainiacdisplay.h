#ifndef BRAINIACDISPLAY_H
#define BRAINIACDISPLAY_H

#include <qglview.h>

class BrainiacDisplay : public QGLView
{
public:
    BrainiacDisplay(QWidget *parent);

protected:
    QGLCamera *m_camera; //!< Camera to display
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

    QPoint m_lastPos; //!< last position of moouse to determine position delta
    bool m_shiftPressed; //!< true, if shift key is pressed
};

#endif // BRAINIACDISPLAY_H
