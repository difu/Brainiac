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
    virtual ~EditorItemConnector();


public slots:
    void updatePosition();

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
    bool m_inverted;
    EditorItem *m_StartItem;
    EditorItem *m_EndItem;
};
#endif // EDITORITEMCONNECTOR_H
