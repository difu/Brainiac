#include "agent.h"
#include "body/body.h"
#include "channel.h"
#include <QDebug>

Agent::Agent(QObject *parent) :
    QObject(parent)
{
    createChannels();
    m_body=new Body(this);
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
    if(m_inputs.contains(name)) {
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

