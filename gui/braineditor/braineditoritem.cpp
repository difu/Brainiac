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
