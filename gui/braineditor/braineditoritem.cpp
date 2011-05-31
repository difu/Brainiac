#include "braineditoritem.h"
#include <QLinearGradient>
#include <QPainter>
#include <QPalette>
#include <QDebug>
#include "core/agent/agentmanager.h"
#include "core/agent/agent.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/fuzzybase.h"

BrainEditorItem::BrainEditorItem(BrainiacGlobals::ItemType type, void *object, quint32 id)  : EditorItem(type, object,id)
{

}

void BrainEditorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    EditorItem::paint(painter, option,widget);
    AgentManager *myManager=(AgentManager*)m_object;
    FuzzyBase *lUnit=myManager->getMasterAgent()->getBrain()->getFuzzy(m_id); //!< \todo use selected Agent

    painter->fillRect( QRectF(relxPos - adjust+5, relyPos - adjust+39,30, 7),Qt::black );
    int tmpResult=0;
    if(lUnit->getMinValue()<0) {
        tmpResult=-30*((lUnit->getResult())/ (lUnit->getMinValue()));
    } else  {
        tmpResult=30*((lUnit->getResult())/ (lUnit->getMaxValue() ));
    }
    //int tmpResult=30*((lUnit->getResult())/ (lUnit->getMaxValue()-lUnit->getMinValue()));
    if( tmpResult>0) {
         painter->fillRect( QRectF(relxPos - adjust+5, relyPos - adjust+39,tmpResult, 7),Qt::red );
     } else {
         tmpResult=-tmpResult;
         painter->fillRect( QRectF(relxPos - adjust+5, relyPos - adjust+39,tmpResult, 7),Qt::blue );
     }
}
