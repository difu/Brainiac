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


#include "editoritem.h"
#include <QLinearGradient>
#include <QPainter>
#include <QPalette>
#include <QGraphicsScene>

#include "gui/editoritemconnector.h"
#include "core/group/group.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/output.h"
//#include "core/agent/body/skeletonnode.h"
#include "core/agent/body/body.h"
#include "core/agent/body/bodymanager.h"
#include <qdrawutil.h>

EditorItem::EditorItem(BrainiacGlobals::ItemType type, void *object,quint32 id)
{
    adjust = 0.5;
    m_type=type;
    m_object=object;
    m_id=id;
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    switch(type) {
    case BrainiacGlobals::AGENT:
        m_symbolPic.load(":/gui/pics/editor_logo_agent.png");
        break;
    case BrainiacGlobals::GROUP:
        m_symbolPic.load(":/gui/pics/editor_logo_group.png");
        break;
    case BrainiacGlobals::OUTPUT:
        m_symbolPic.load(":/gui/pics/editor_logo_output.png");
        break;
    case BrainiacGlobals::INPUT:
        m_symbolPic.load(":/gui/pics/editor_logo_input.png");
        break;
    case BrainiacGlobals::NOISE:
        m_symbolPic.load(":/gui/pics/editor_logo_noise.png");
        break;
    case BrainiacGlobals::AND:
        m_symbolPic.load(":/gui/pics/editor_logo_and.png");
        break;
    case BrainiacGlobals::OR:
        m_symbolPic.load(":/gui/pics/editor_logo_or.png");
        break;
    case BrainiacGlobals::DEFUZZ:
        m_symbolPic.load(":/gui/pics/editor_logo_defuzz.png");
        break;
    case BrainiacGlobals::FUZZ:
        m_symbolPic.load(":/gui/pics/editor_logo_fuzz.png");
        break;
    case BrainiacGlobals::TIMER:
        m_symbolPic.load(":/gui/pics/editor_logo_timer.png");
        break;
    default:
        qDebug() << __PRETTY_FUNCTION__ << "Wrong label parameter!";
    }
}

void EditorItem::addConnector(EditorItemConnector *connector){
    m_connectors.append(connector);
}

QRectF EditorItem::boundingRect() const
{
    return QRectF(relxPos - adjust*15, relyPos - adjust*15,WIDTH + adjust*15, HEIGHT + adjust*15);
}

QVariant EditorItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (EditorItemConnector *connector, m_connectors) {
            connector->updatePosition();
        }
        // value is the new position.
        QPoint newPos = value.toPoint();
        int x=((newPos.x())/_raster);
        int y=((newPos.y())/_raster);
        newPos.setX(x*_raster);
        newPos.setY(y*_raster);
        return newPos;

    }
    return QGraphicsItem::itemChange(change, value);
}

void EditorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QLinearGradient myGrad(relxPos, relyPos, relxPos+WIDTH, relyPos+HEIGHT);
    myGrad.setColorAt(0.5, Qt::gray);
    myGrad.setColorAt(1, Qt::white);
    QBrush myBrush( myGrad );
    qDrawShadePanel(painter, relxPos, relyPos, WIDTH, HEIGHT, QPalette(QColor(30, 30, 30, 127)),true,4, &myBrush);
    painter->setPen(Qt::black);
    if(m_type==BrainiacGlobals::GROUP) {
        Group *myGroup=(Group*)m_object;
        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myGroup->getName());
    } else if(m_type==BrainiacGlobals::AGENT) {
        AgentManager *myManager=(AgentManager*)m_object;
        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myManager->getName());
    } else if(m_type==BrainiacGlobals::OUTPUT) {
        AgentManager *myManager=(AgentManager*)m_object;
        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myManager->getMasterAgent()->getBrain()->getFuzzy(m_id)->getName());
    } else if(m_type==BrainiacGlobals::INPUT) {
        AgentManager *myManager=(AgentManager*)m_object;
        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myManager->getMasterAgent()->getBrain()->getFuzzy(m_id)->getName());
    } else if(m_type==BrainiacGlobals::NOISE) {
        AgentManager *myManager=(AgentManager*)m_object;
        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myManager->getMasterAgent()->getBrain()->getFuzzy(m_id)->getName());
    } else if(m_type==BrainiacGlobals::AND) {
        AgentManager *myManager=(AgentManager*)m_object;
        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myManager->getMasterAgent()->getBrain()->getFuzzy(m_id)->getName());
    } else if(m_type==BrainiacGlobals::OR) {
        AgentManager *myManager=(AgentManager*)m_object;
        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myManager->getMasterAgent()->getBrain()->getFuzzy(m_id)->getName());
    } else if(m_type==BrainiacGlobals::DEFUZZ) {
        AgentManager *myManager=(AgentManager*)m_object;
        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myManager->getMasterAgent()->getBrain()->getFuzzy(m_id)->getName());
    } else if(m_type==BrainiacGlobals::FUZZ) {
        AgentManager *myManager=(AgentManager*)m_object;
        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myManager->getMasterAgent()->getBrain()->getFuzzy(m_id)->getName());
    } else if(m_type==BrainiacGlobals::TIMER) {
        AgentManager *myManager=(AgentManager*)m_object;
        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myManager->getMasterAgent()->getBrain()->getFuzzy(m_id)->getName());
    } else if(m_type==BrainiacGlobals::CUBE) {
        AgentManager *myManager=(AgentManager*)m_object;
        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myManager->getBodyManager()->getSegment(m_id).getName());
    } else if(m_type==BrainiacGlobals::SPHERE) {
        AgentManager *myManager=(AgentManager*)m_object;
        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myManager->getBodyManager()->getSegment(m_id).getName());
    } else if(m_type==BrainiacGlobals::TUBE) {
        AgentManager *myManager=(AgentManager*)m_object;
        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myManager->getBodyManager()->getSegment(m_id).getName());
    }
    painter->drawImage(QPoint(relxPos+4,relyPos+6),m_symbolPic);

    if( this->isSelected() ) {
        painter->setPen(Qt::red);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect( QRectF(relxPos - adjust, relyPos - adjust,WIDTH + adjust, HEIGHT + adjust) );
    }
}

QPainterPath EditorItem::shape() const
{
    QPainterPath path;
    path.addRect(relxPos, relyPos, WIDTH, HEIGHT);
    return path;
}

void EditorItem::removeConnector(EditorItemConnector *connector)
{
    int index = m_connectors.indexOf(connector);

    if (index != -1)
        m_connectors.removeAt(index);
}

void EditorItem::removeConnectors()
{
    foreach (EditorItemConnector *connector, m_connectors) {
        connector->startItem()->removeConnector(connector);
        connector->endItem()->removeConnector(connector);
        scene()->removeItem(connector);
        delete connector;
    }
}
