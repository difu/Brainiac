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


#include "editorgraphicsview.h"
#include "editorbase.h"
#include <QMouseEvent>
#include <QDebug>

#include <QScrollBar>

EditorGraphicsView::EditorGraphicsView():QGraphicsView()
{
    setDragMode(QGraphicsView::RubberBandDrag);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_rightButtonPressed=m_middleButtonPressed=false;
    //setDragMode(QGraphicsView::ScrollHandDrag);
//    connect(this->horizontalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(sliderMoved(int)));
//    connect(this->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(sliderMoved(int)));

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
    if(m_rightButtonPressed) {
        if(m_oldMousePos.x()>mouseEvent->pos().x()) {
            this->scale(0.9f,0.9f);
        } else {
            this->scale(1.1f,1.1f);
            QMatrix m=this->matrix();
            if(m.m11()>1.0f || m.m22() >1.0f) {
                m.reset();
                setMatrix(m);
            }
        }

    } else if(m_middleButtonPressed) {

        int oldXSBVal=this->horizontalScrollBar()->value();
        int oldYSBVal=this->verticalScrollBar()->value();
        this->horizontalScrollBar()->setValue(oldXSBVal-(mouseEvent->pos()-m_oldMousePos).x());
        this->verticalScrollBar()->setValue(oldYSBVal-(mouseEvent->pos()-m_oldMousePos).y());
    } else {
        QGraphicsView::mouseMoveEvent(mouseEvent);
    }
    m_oldMousePos=mouseEvent->pos();
}

void EditorGraphicsView::mousePressEvent(QMouseEvent *mouseEvent)
{
    QGraphicsView::mousePressEvent(mouseEvent);
    if(mouseEvent->button()==Qt::RightButton) {
        m_rightButtonPressed=true;
    } else if(mouseEvent->button()==Qt::MiddleButton) {
        m_middleButtonPressed=true;
    }
    m_oldMousePos=mouseEvent->pos();

}

void EditorGraphicsView::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    QGraphicsView::mouseReleaseEvent(mouseEvent);
    if(mouseEvent->button()==Qt::RightButton) {
        m_rightButtonPressed=false;
    } else if(mouseEvent->button()==Qt::MiddleButton) {
        m_middleButtonPressed=false;
    }
    EditorBase *editor=dynamic_cast<EditorBase *> (this->scene());
    if(editor) {
        editor->setViewMatrixAndViewTranslation(this->matrix(),this->horizontalScrollBar()->value(),this->verticalScrollBar()->value());
    }
//    qDebug() << __PRETTY_FUNCTION__ << this->matrix() << this->horizontalScrollBar()->value() << this->verticalScrollBar()->value();

}

void EditorGraphicsView::setScene(EditorBase *scene)
{
    if(scene) {
        if(scene->viewHasNeverBeenSet()) {
            setMatrix(QMatrix());
            centerOn(scene->width()/2,scene->height()/2);
        } else {
            setMatrix(scene->getViewMatrix());
            this->horizontalScrollBar()->setValue(scene->getViewXtrans());
            this->verticalScrollBar()->setValue(scene->getViewYtrans());
//            qDebug() << __PRETTY_FUNCTION__ << this->matrix() << this->horizontalScrollBar()->value() << this->verticalScrollBar()->value();
        }
    }
    QGraphicsView::setScene((QGraphicsScene*) scene);
}

void EditorGraphicsView::sliderMoved(int value)
{
    Q_UNUSED(value);
    EditorBase *editor=dynamic_cast<EditorBase *> (this->scene());
    if(editor) {
        editor->setViewMatrixAndViewTranslation(this->matrix(),this->horizontalScrollBar()->value(),this->verticalScrollBar()->value());
//        qDebug() << __PRETTY_FUNCTION__ << this->matrix() << this->horizontalScrollBar()->value() << this->verticalScrollBar()->value();
    }
}
