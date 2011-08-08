#include "braineditoritem.h"
#include <QLinearGradient>
#include <QPainter>
#include <QPalette>
#include <QDebug>
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/fuzzybase.h"
#include "core/agent/brain/fuzzyand.h"
#include "braineditor.h"

BrainEditorItem::BrainEditorItem(BrainiacGlobals::ItemType type, void *object, quint32 id)  : EditorItem(type, object,id)
{
    if(type==BrainiacGlobals::AND || type==BrainiacGlobals::OR) {
        m_featurePic.load(":/gui/pics/editor_logo_sound_rule.png");
    }
}

void BrainEditorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    EditorItem::paint(painter, option,widget);
    AgentManager *myManager=(AgentManager*)m_object;
    QGraphicsScene *scene=this->scene();
    FuzzyBase *lUnit;
    if(!((BrainEditor*)scene)->getSelectedAgent()) {
        lUnit=myManager->getMasterAgent()->getBrain()->getFuzzy(m_id);
    } else {
        lUnit=((BrainEditor*)scene)->getSelectedAgent()->getBrain()->getFuzzy(m_id);
    }

    painter->fillRect( QRectF(relxPos - adjust+5, relyPos - adjust+39,30, 7),Qt::black );

    int tmpResult=0;
    if(lUnit->getMinValue()<0) {
        tmpResult=-30*((lUnit->getResult())/ (lUnit->getMinValue()));
    } else  {
        tmpResult=30*((lUnit->getResult())/ (lUnit->getMaxValue() ));
    }

    if( tmpResult>0) {
         painter->fillRect( QRectF(relxPos - adjust+5, relyPos - adjust+39,tmpResult, 7),Qt::red );
     } else {
         tmpResult=-tmpResult;
         painter->fillRect( QRectF(relxPos - adjust+5, relyPos - adjust+39,tmpResult, 7),Qt::blue );
     }
    if(lUnit->getType()==FuzzyBase::AND) {
        FuzzyAnd *myAnd=(FuzzyAnd *)lUnit;
        if(myAnd->isSoundRule()) {
            painter->drawImage(relxPos+22,relyPos+5,m_featurePic);
        }
    }
}
