#include "agent.h"
#include "body/body.h"
#include "core/brainiacglobals.h"
#include "channel.h"
#include "core/scene.h"
#include <QDebug>

Agent::Agent(Scene *scene, quint32 id,  QObject *parent) :
    QObject(parent)
{
    m_id=id;
    createChannels();
    m_body=new Body(this);
    m_scene=scene;
}

Agent::~Agent() {
    delete m_body;
    foreach(Channel *chan,m_inputs) {
        delete chan;
    }
    m_inputs.clear();
    foreach(Channel *chan,m_outputs) {
        delete chan;
    }
    m_outputs.clear();
}

/** \brief adds an input channel

                adds an input channel to the agent.
                If channel already exists nothing is added
                \return true, if adding was successful, false if not

**/
bool Agent::addInputChannel(Channel *channel, QString name)
{
    if(m_inputs.contains(name)) {
        qDebug() << __PRETTY_FUNCTION__ << "Channel " << name << "already exists!";
        return false;
    } else {
        m_inputs.insert(name,channel);
        return true;
    }
}

/** \brief adds an output channel

                adds an output channel to the agent.
                If channel already exists nothing is added
                \return true, if adding was successful, false if not

**/
bool Agent::addOutputChannel(Channel *channel, QString name)
{
    if(m_outputs.contains(name)) {
        qDebug() << __PRETTY_FUNCTION__ << "Channel " << name << "already exists!";
        return false;
    } else {
        m_outputs.insert(name,channel);
        return true;
    }
}

void Agent::createChannels()
{
    m_tx=new Channel();
    addInputChannel(m_tx,"tx");
    addOutputChannel(m_tx,"tx");

    m_ty=new Channel();
    addInputChannel(m_ty,"ty");
    addOutputChannel(m_ty,"ty");

    m_tz=new Channel();
    addInputChannel(m_tz,"tz");
    addOutputChannel(m_tz,"tz");

}

/** \brief deletes channel

                the channel is deleted both from inputs and outputs.
                also its destructor is invoked

**/
void Agent::deleteChannel(Channel *channel)
{
    m_inputs.remove(m_inputs.key(channel));
    m_outputs.remove(m_inputs.key(channel));
    delete channel;
}

Body* Agent::getBody()
{
    return m_body;
}

quint32 Agent::getId() {
    return m_id;
}

void Agent::setRotation(qreal x, qreal y, qreal z)
{
    BrainiacGlobals::normalizeAngle(&x);
    BrainiacGlobals::normalizeAngle(&y);
    BrainiacGlobals::normalizeAngle(&z);
    m_rotation.setX(x);
    m_rotation.setY(y);
    m_rotation.setZ(z);
}

void Agent::setTranslation(qreal x, qreal y, qreal z)
{
    m_position.setX(x);
    m_position.setY(y);
    m_position.setZ(z);
}

void Agent::setRestRotation(qreal x, qreal y, qreal z)
{
    BrainiacGlobals::normalizeAngle(&x);
    BrainiacGlobals::normalizeAngle(&y);
    BrainiacGlobals::normalizeAngle(&z);
    m_restRotation.setX(x);
    m_restRotation.setY(y);
    m_restRotation.setZ(z);
}

void Agent::setRestTranslation(qreal x, qreal y, qreal z)
{
    m_restPosition.setX(x);
    m_restPosition.setY(y);
    m_restPosition.setZ(z);
}
