#include "loopeditorscene.h"
#include <QGraphicsView>
#include <QColor>
#include "core/agent/body/animation/modifiableanimation.h"
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

void LoopEditorScene::updateTime(qreal time)
{
    setTimeCursor(time);
}
