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


#ifndef EDITORGRAPHICSVIEW_H
#define EDITORGRAPHICSVIEW_H

#include <QGraphicsView>

class EditorBase;

/**
 * @brief The view of all editors in of the MainWindow´s central widget
 *
 * @class EditorGraphicsView editorgraphicsview.h "gui/editorgraphicsview.h"
 */
class EditorGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @fn EditorGraphicsView
 */
    EditorGraphicsView();
    /**
     * @brief sets the scene for this EditorGraphicsView
     *  the views matrix and position of the scrollbars are also set
     *
     * @fn setScene
     * @bug mouse wheel scrolling and translation of scene is not stored properly
     * @param scene the EditorBase scene to be set
     */
    void setScene(EditorBase *scene);

protected:
    /**
     * @brief
     *
     * @fn keyPressEvent
     * @param keyEvent
     */
    void keyPressEvent (QKeyEvent *keyEvent);
    /**
     * @brief
     *
     * @fn keyReleaseEvent
     * @param keyEvent
     */
    void keyReleaseEvent(QKeyEvent *keyEvent);
    /**
     * @brief
     *
     * @fn mousePressEvent
     * @param mouseEvent
     */
    void mousePressEvent(QMouseEvent *mouseEvent);
    /**
     * @brief
     *
     * @fn mouseMoveEvent
     * @param mouseEvent
     */
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    /**
     * @brief
     *
     * @fn mouseReleaseEvent
     * @param mouseEvent
     */
    void mouseReleaseEvent(QMouseEvent *mouseEvent);
    bool m_rightButtonPressed; /**< true, if right mouse button is pressed */
    bool m_middleButtonPressed; /**< true, if middle mouse button is pressed */
    QPoint m_oldMousePos; /**< the old mouse position */

protected slots:
    void sliderMoved(int value);
};

#endif // EDITORGRAPHICSVIEW_H
