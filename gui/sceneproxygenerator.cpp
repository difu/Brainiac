#include "sceneproxygenerator.h"
#include "gui/scenedisplay.h"
#include "core/scene.h"
#include "core/simulation.h"

SceneProxyGenerator::SceneProxyGenerator(QObject *parent) :
    QObject(parent),
    m_dirty(false)
{
    connect(getSceneDisplay()->scene(),SIGNAL(sceneContentChanged()),this,SLOT(scheduleUpdate()));
    connect(getSceneDisplay()->scene()->getSimulation(),SIGNAL(stopped()),this,SLOT(scheduleUpdate()));
    startTimer(3000);
}


SceneDisplay* SceneProxyGenerator::getSceneDisplay() const
{
    SceneDisplay* sd=qobject_cast<SceneDisplay *>(parent());
    if(!sd) {
        qCritical() << __PRETTY_FUNCTION__ << "Could not cast!";
    }
    return sd;
}
void SceneProxyGenerator::scheduleUpdate()
{
    qDebug() << __PRETTY_FUNCTION__ << "update scheduled";
    m_dirty=true;
}

void SceneProxyGenerator::timerEvent(QTimerEvent *event)
{
    if(m_dirty) {
        qDebug() << __PRETTY_FUNCTION__ << "bla______";
    }
}
