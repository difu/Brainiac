#include "editoritem.h"
#include <QLinearGradient>
#include <QPainter>
#include <QPalette>

#include "core/group/group.h"
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/output.h"

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
    default:
        qDebug() << __PRETTY_FUNCTION__ << "Wrong label parameter!";
    }
}

QRectF EditorItem::boundingRect() const
{
    return QRectF(relxPos - adjust*15, relyPos - adjust*15,_width + adjust*15, _height + adjust*15);
}

QVariant EditorItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
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
    QLinearGradient myGrad(relxPos, relyPos, relxPos+_width, relyPos+_height);
    myGrad.setColorAt(0.5, Qt::gray);
    myGrad.setColorAt(1, Qt::white);
    QBrush myBrush( myGrad );
    qDrawShadePanel(painter, relxPos, relyPos, _width, _height, QPalette(QColor(30, 30, 30, 127)),true,4, &myBrush);
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
    }
    painter->drawImage(QPoint(relxPos+4,relyPos+6),m_symbolPic);

    if( this->isSelected() ) {
        painter->setPen(Qt::red);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect( QRectF(relxPos - adjust, relyPos - adjust,_width + adjust, _height + adjust) );
    }
}

QPainterPath EditorItem::shape() const
{
    QPainterPath path;
    path.addRect(relxPos, relyPos, _width, _height);
    return path;
}

