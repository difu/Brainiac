// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


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


    QPoint m_lastPos; //!< last position of mouse to determine position delta
    bool m_shiftPressed; //!< true, if shift key is pressed
    bool m_showCoordCross;  /**< true, if worldspace coord cross should be drawn */
    bool m_showAgentBoneCoordCrosses;
    bool m_renderSilhouettes;
    bool m_renderSkeleton;
};

#endif // BRAINIACDISPLAY_H
