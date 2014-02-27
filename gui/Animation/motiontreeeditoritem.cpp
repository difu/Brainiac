#include "motiontreeeditoritem.h"

#include <QLinearGradient>
#include <QPainter>
#include <QPalette>
#include <qdrawutil.h>

#include "core/agent/body/animation/motiontreetransition.h"


MotionTreeEditorItem::MotionTreeEditorItem(BrainiacGlobals::ItemType type, void *object) : EditorItem(type, object,0)
{
}


void MotionTreeEditorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(m_type==BrainiacGlobals::ACTION) {
        QLinearGradient myGrad(relxPos, relyPos, relxPos+WIDTH, relyPos+HEIGHT);

        QBrush myBrush( myGrad );
        myGrad.setColorAt(0, Qt::white);
        myGrad.setColorAt(1, Qt::white);
        painter->setPen(Qt::white);
        painter->setBrush(myBrush);
        painter->drawEllipse(QRect(relxPos,relyPos,WIDTH,HEIGHT));
    } else {
        QLinearGradient myGrad(relxPos, relyPos, relxPos+WIDTH, relyPos+HEIGHT);
        MotionTreeTransition *trans=(MotionTreeTransition *) (m_object);
        if(trans) {
            QColor col=trans->getColor();
            myGrad.setColorAt(0, col);
            myGrad.setColorAt(1, col);
        }
        QBrush myBrush( myGrad );
        painter->setPen(Qt::white);
        painter->setBrush(myBrush);
        painter->drawRect(QRect(relxPos,relyPos,WIDTH,HEIGHT));
    }

//    qDrawShadePanel(painter, relxPos, relyPos, WIDTH, HEIGHT, QPalette(QColor(30, 30, 30, 127)),true,4, &myBrush);

    painter->setPen(Qt::black);
//    if(m_type==BrainiacGlobals::GROUP) {
//        Group *myGroup=(Group*)m_object;
//        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myGroup->getName());
//    } else if(m_type==BrainiacGlobals::AGENT) {
//        AgentManager *myManager=(AgentManager*)m_object;
//        painter->drawText(relxPos - adjust+40,relyPos - adjust+10,75,50,Qt::TextWordWrap|Qt::AlignHCenter,myManager->getName());
//    }

    //painter->drawImage(QPoint(relxPos+4,relyPos+6),m_symbolPic);

    if( this->isSelected() ) {
        painter->setPen(Qt::red);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect( QRectF(relxPos - adjust, relyPos - adjust,WIDTH + adjust, HEIGHT + adjust) );
    }
}
