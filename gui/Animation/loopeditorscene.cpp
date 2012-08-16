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


#include "loopeditorscene.h"
#include <QGraphicsView>
#include <QColor>
#include "core/agent/body/animation/modifiableanimation.h"
#include "core/agent/body/animation/animationcurve.h"
#include "gui/Animation/editorlineitem.h"

LoopEditorScene::LoopEditorScene() : QGraphicsScene(0),m_animation(0)
{
    QPen pen;
    m_timeCurserItem=new EditorLineItem(this);
    pen.setColor(QColor(255,0,0));
    m_timeCurserItem->setPen(pen);

    m_startCurserItem=new EditorLineItem(this);
    pen.setColor(QColor(0,255,0));
    m_startCurserItem->setPen(pen);

    m_endCurserItem=new EditorLineItem(this);
    pen.setColor(QColor(0,100,240));
    m_endCurserItem->setPen(pen);

    m_crossFadeEndItem=new EditorLineItem(this);
    pen.setColor(QColor(100,100,100));
    m_crossFadeEndItem->setPen(pen);

    m_crossFadeStartItem=new EditorLineItem(this);
    m_crossFadeStartItem->setPen(pen);
}

qreal LoopEditorScene::mapCurveRangeToHeight(qreal max, qreal min, qreal value) const
{
    Q_ASSERT(max>=min);
    qreal height=this->height();
    if(qFuzzyCompare(max+1.0f,min+1.0f)) {
        return height/2.0f;
    }
    return height*BrainiacGlobals::norm(min,max,value);
}

qreal LoopEditorScene::mapTimeToWidth(qreal time) const
{
    return sceneRect().width()*(time/m_animation->getLength(true));
}

qreal LoopEditorScene::mapWidthToTime(qreal width) const
{
    return (width/sceneRect().width()*m_animation->getLength(true));
}

void LoopEditorScene::setAnimation(ModifiableAnimation *animation)
{
    QGraphicsView *view;
    if(views().count()>0) {
        view=views().first();
    } else {
        qWarning() << __PRETTY_FUNCTION__ << "No Graphics view available!";
        return;
    }
    QMutexLocker locker(&m_animChangeMutex);
    Q_UNUSED(locker);
    m_animation=animation;
    setSceneRect(0,0,view->geometry().width()-20,view->geometry().height()-20);
    m_timeCurserItem->setPos(3,0);
    update();
}

void LoopEditorScene::setAnimationCurveNames(QList<QString> curveNames)
{
    m_curveItemNames=curveNames;
    this->updateCurves();
}

void LoopEditorScene::setTimeCursor(qreal time)
{
//    QMutexLocker locker(&m_animChangeMutex);
//    Q_UNUSED(locker);
    time=qAbs(time);
    qreal animLength=m_animation->getLength(true);
    if(animLength>0)
        m_timeCurserItem->setPos(mapTimeToWidth(time),0);
    else m_timeCurserItem->setPos(0,0);
}

void LoopEditorScene::update()
{
//    QMutexLocker locker(&m_animChangeMutex);
//    Q_UNUSED(locker);
    m_startCurserItem->setPos(mapTimeToWidth(m_animation->getStartTime()),0);
    m_endCurserItem->setPos(mapTimeToWidth(m_animation->getEndTime()),0);
    if(!qFuzzyCompare(m_animation->getCrossFade(),(qreal)0.0f)) {
        m_crossFadeEndItem->show();
        m_crossFadeStartItem->show();
        m_crossFadeStartItem->setPos(mapTimeToWidth(m_animation->getStartTime()+m_animation->getCrossFade()),0);
        m_crossFadeEndItem->setPos(mapTimeToWidth(m_animation->getEndTime()+m_animation->getCrossFade()),0);
    } else {
        m_crossFadeEndItem->hide();
        m_crossFadeStartItem->hide();
    }

}

void LoopEditorScene::updateCurves()
{
    foreach(QList< QGraphicsItem *> gItems,m_curveItems)
    {
        foreach(QGraphicsItem *item,gItems) {
            this->removeItem(item);
            delete(item);
        }
    }
    m_curveItems.clear();
    int numberOfCurves=m_curveItemNames.count()+1; // +1 to avoid black color
    int curveNo=1; // +1 to avoid black color
    foreach(QString cn,m_curveItemNames) {
        AnimationCurve *curve=m_animation->curves().value(cn,0);
        if(curve) {
            qreal minValue=curve->getMinValue();
            qreal maxValue=curve->getMaxValue();
            QList< QGraphicsItem *> gItems;
            QBrush brush(BrainiacGlobals::getColorFromBrainiacColorValue((qreal)curveNo/(qreal)(numberOfCurves)));
            foreach(QVector2D kf,curve->keyFrames()) {
                QGraphicsRectItem *item=new QGraphicsRectItem(-1,-1,1,1);
                item->setPos(mapTimeToWidth(kf.x()),mapCurveRangeToHeight(maxValue,minValue,kf.y()));
                item->setBrush(brush);
                this->addItem(item);
                gItems.append(item);
            }
            curveNo++;
            m_curveItems.insert(cn,gItems);
        }
    }
}

void LoopEditorScene::updateTime(qreal time)
{
    setTimeCursor(time);
}
