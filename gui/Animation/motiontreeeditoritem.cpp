#include "motiontreeeditoritem.h"

#include <QLinearGradient>
#include <QPainter>
#include <QPalette>
#include <qdrawutil.h>

#include "core/agent/body/animation/motiontreetransition.h"
#include "core/agent/body/animation/motiontreeaction.h"
#include "core/agent/body/animation/motiontree.h"


MotionTreeEditorItem::MotionTreeEditorItem(BrainiacGlobals::ItemType type, void *object) : EditorItem(type, object,0)
{
}


void MotionTreeEditorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(m_type==BrainiacGlobals::ACTION) {
        MotionTreeAction *action=(MotionTreeAction *) m_object;
        QLinearGradient myGrad(relxPos, relyPos, relxPos+WIDTH, relyPos+HEIGHT);

        QBrush myBrush( myGrad );
        myGrad.setColorAt(0, Qt::white);
        myGrad.setColorAt(1, Qt::white);
        if(action->isTreeDefaultAction()) {
            painter->setPen(Qt::red);
        } else {
            painter->setPen(Qt::white);
        }
        painter->setBrush(myBrush);
        painter->drawEllipse(QRect(relxPos,relyPos,WIDTH,HEIGHT));
        painter->setPen(Qt::green);
        painter->drawText(relxPos - adjust+10,relyPos - adjust+15,85,50,Qt::TextWordWrap|Qt::AlignHCenter,(action->getName()));

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
        painter->drawText(relxPos - adjust+10,relyPos - adjust+15,85,50,Qt::TextWordWrap|Qt::AlignHCenter,(trans->getName()));
    }

    painter->setPen(Qt::black);

    if( this->isSelected() ) {
        painter->setPen(Qt::red);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect( QRectF(relxPos - adjust, relyPos - adjust,WIDTH + adjust, HEIGHT + adjust) );
    }
}

MotionTreeEditorItem::~MotionTreeEditorItem()
{
    removeConnectors();
}
