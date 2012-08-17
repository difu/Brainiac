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


#ifndef EDITORBASE_H
#define EDITORBASE_H

#include <QGraphicsScene>

#include "gui/itemeditorwidgetsbase.h"

class Scene;


/** \brief  Base class for item based editors

        All item based editors (these which appear in center region of the main window) must subclass this

**/
class EditorBase : public QGraphicsScene
{
    Q_OBJECT
public:
    EditorBase(Scene *scene);

    /**
     * @brief sets the view matrix (scale) and the translation (the h/v sliders) of this scene when displayed in the EditorGraphicsView
     *
     * @fn setViewMatrixAndViewTranslation
     * @sa EditorGraphicsView
     * @param m the matrix containing the scale
     * @param x the value of the horizontal slider
     * @param y the value of the vertical slider
     */
    void setViewMatrixAndViewTranslation(QMatrix m, int x, int y ) { m_viewMatrix=m; m_viewXtrans=x;m_viewYtrans=y;m_viewHasNeverBeenSet=false;}
    int getViewXtrans() const {return m_viewXtrans; }
    int getViewYtrans() const {return m_viewYtrans; }
    QMatrix getViewMatrix() const { return m_viewMatrix; }

    /**
     * @brief returns if the scale matrix etc have never been set before
     * calling setViewMatrixAndViewTranslation() will return this function always true
     * @sa setViewMatrixAndViewTranslation
     *
     * @fn viewHasNeverBeenSet
     * @return bool
     */
    bool viewHasNeverBeenSet() const { return m_viewHasNeverBeenSet;}
protected:
    Scene *m_scene;
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    QMatrix m_viewMatrix;
    int m_viewXtrans;
    int m_viewYtrans;
    bool m_viewHasNeverBeenSet;
signals:
    void itemClicked(ItemEditorWidgetsBase::editMessage msg);
};

#endif // EDITORBASE_H
