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


#include "editorlabel.h"
#include <QtWidgets>
#include <QDebug>


EditorLabel::EditorLabel(BrainiacGlobals::ItemType labelType) :
    QLabel()
{
    m_type=labelType;
    switch(m_type) {
    case BrainiacGlobals::AND:
        setPixmap(QPixmap(":/gui/pics/editor_logo_and.png"));
        break;
    case BrainiacGlobals::OR:
        setPixmap(QPixmap(":/gui/pics/editor_logo_or.png"));
        break;
    case BrainiacGlobals::FUZZ:
        setPixmap(QPixmap(":/gui/pics/editor_logo_fuzz.png"));
        break;
    case BrainiacGlobals::DEFUZZ:
        setPixmap(QPixmap(":/gui/pics/editor_logo_defuzz.png"));
        break;
    case BrainiacGlobals::TIMER:
        setPixmap(QPixmap(":/gui/pics/editor_logo_timer.png"));
        break;
    case BrainiacGlobals::NOISE:
        setPixmap(QPixmap(":/gui/pics/editor_logo_noise.png"));
        break;
    case BrainiacGlobals::OUTPUT:
        setPixmap(QPixmap(":/gui/pics/editor_logo_output.png"));
        break;
    case BrainiacGlobals::INPUT:
        setPixmap(QPixmap(":/gui/pics/editor_logo_input.png"));
        break;
    case BrainiacGlobals::AGENT:
        setPixmap(QPixmap(":/gui/pics/editor_logo_agent.png"));
        break;
    case BrainiacGlobals::GROUP:
        setPixmap(QPixmap(":/gui/pics/editor_logo_group.png"));
        break;
    default:
        qWarning() << __PRETTY_FUNCTION__ << "Wrong label parameter or label not yet implemented!";
    }
    setMouseTracking(true);
}

void EditorLabel::mousePressEvent(QMouseEvent *ev)
{
    QLabel::mousePressEvent(ev);
    QDrag *drag=new QDrag(this);
    QPixmap pixmap;
    switch(m_type) {
    case BrainiacGlobals::AND:
        pixmap.load(":/gui/pics/editor_logo_and.png");
        break;
    case BrainiacGlobals::OR:
        pixmap.load(":/gui/pics/editor_logo_or.png");
        break;
    case BrainiacGlobals::FUZZ:
        pixmap.load(":/gui/pics/editor_logo_fuzz.png");
        break;
    case BrainiacGlobals::DEFUZZ:
        pixmap.load(":/gui/pics/editor_logo_defuzz.png");
        break;
    case BrainiacGlobals::TIMER:
        pixmap.load(":/gui/pics/editor_logo_timer.png");
        break;
    case BrainiacGlobals::NOISE:
        pixmap.load(":/gui/pics/editor_logo_noise.png");
        break;
    case BrainiacGlobals::OUTPUT:
        pixmap.load(":/gui/pics/editor_logo_output.png");
        break;
    case BrainiacGlobals::INPUT:
        pixmap.load(":/gui/pics/editor_logo_input.png");
        break;
    case BrainiacGlobals::AGENT:
        pixmap.load(":/gui/pics/editor_logo_agent.png");
        break;
    case BrainiacGlobals::GROUP:
        pixmap.load(":/gui/pics/editor_logo_group.png");
        break;
    default:
        qDebug() << __PRETTY_FUNCTION__ << "Wrong label parameter or label not yet implemented!";
    }

    drag->setPixmap(pixmap);
    QMimeData *mimeData = new QMimeData;
    drag->setHotSpot(QPoint(drag->pixmap().width()/2,
                            drag->pixmap().height()));
    mimeData->setText(QString("AddLogicType "+QString::number(m_type) ));
    drag->setMimeData(mimeData);
    drag->start();
}
