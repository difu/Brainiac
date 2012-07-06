#ifndef BRAINIACDISPLAY_H
#define BRAINIACDISPLAY_H

#include <qglview.h>

/**
 * @brief Baseclass of all OpenGL Views of Brainiac
 *
 * This provides common functions for all Brainiac OpenGL Views
 * @class BrainiacDisplay brainiacdisplay.h "gui/brainiacdisplay.h"
 */
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
    virtual void paintGL(QGLPainter *painter);


    QPoint m_lastPos; //!< last position of moouse to determine position delta
    bool m_shiftPressed; //!< true, if shift key is pressed
    bool m_showCoordCross;
};

#endif // BRAINIACDISPLAY_H
