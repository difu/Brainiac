#include "agent.h"
#include "brain/brain.h"
#include "body/body.h"
#include "body/segment.h"
#include "body/sphere.h"
#include "core/brainiacglobals.h"
#include "channel.h"
#include "core/scene.h"
#include <QtOpenGL>
#include <glu.h>
#include <GLUT/glut.h>
#include <QDebug>

Agent::Agent(Scene *scene, quint32 id) :
    QObject()
{
    m_id=id;
    createChannels();
    m_body=new Body(this,0);
    m_brain=new Brain(this,0);
    m_scene=scene;
}

Agent::Agent(Agent *agent, quint32 id)  :
    QObject()
{
    m_id=id;
    createChannels();
    m_body=new Body(this,agent->getBody());
    m_brain=new Brain(this,agent->getBrain());
    m_scene=agent->getScene();
}

Agent::~Agent() {
    delete m_body;
    delete m_brain;
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
                \param  channel pointer to output channel
                @param name name of the channel
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

void Agent::addOutputFuzz(quint32 id, QString name, QString channel)
{
    m_brain->addOutputFuzz(id, name, channel);
}

void Agent::addInputFuzz(quint32 id, QString name, QString channel)
{
    m_brain->addInputFuzz(id, name, channel);
}

void Agent::addNoiseFuzz(quint32 id, QString name, qreal rate)
{
    m_brain->addNoiseFuzz(id, name,rate);
}

/** \brief advances this agent

                calling this function makes the agent go one step further in time
                Typically this function is called from the Simulation class

                It queries infos of all agents in the scene and the scene´s environment
                It triggers its brain to process

                The changes have to be written down after all agents have calculated their
                new values via advanceCommit()

                @sa Simulation
                @sa Brain
                @sa Agent::advanceCommit()

**/
void Agent::advance()
{
    //qDebug() << __PRETTY_FUNCTION__<< "Agent " << m_id;
    // Process movement
    m_newRotation.setX(m_rotation.x()+ m_rx->getValue());
    m_newRotation.setY(m_rotation.y()+ m_ry->getValue());
    m_newRotation.setZ(m_rotation.z()+ m_rz->getValue());

    m_newPosition.setX(m_position.x()+m_tz->getValue()*BrainiacGlobals::sinGrad(m_newRotation.y()));
    m_newPosition.setY(m_position.y()); //!< @todo Implement this!
    m_newPosition.setZ(m_position.z()+m_tz->getValue()*BrainiacGlobals::cosGrad(m_newRotation.y()));
}

/** \brief commits all changes calculated by advance

                all calculated channels etc are written down and are "baked"
                @sa Agent::advance()

**/
void Agent::advanceCommit()
{
    foreach(Channel *channel, m_inputs) {
        channel->advance();
    }
    foreach(Channel *channel, m_outputs ) {
        channel->advance();
    }
    m_position=m_newPosition;
    m_rotation=m_newRotation;
}

void Agent::createChannels()
{
    m_rx=new Channel();
    addInputChannel(m_rx,"rx");
    addOutputChannel(m_rx,"rx");

    m_ry=new Channel();
    addInputChannel(m_ry,"ry");
    addOutputChannel(m_ry,"ry");

    m_rz=new Channel();
    addInputChannel(m_rz,"rz");
    addOutputChannel(m_rz,"rz");

    m_tx=new Channel();
    addInputChannel(m_tx,"tx");
    addOutputChannel(m_tx,"tx");

    m_ty=new Channel();
    addInputChannel(m_ty,"ty");
    addOutputChannel(m_ty,"ty");

    m_tz=new Channel();
    addInputChannel(m_tz,"tz");
    addOutputChannel(m_tz,"tz");

    m_color=new Channel(0,1,0);
    addInputChannel(m_color,"color");
    addOutputChannel(m_color,"color");

}

/** \brief deletes channel

                the channel is deleted both from inputs and outputs.
                also its destructor is invoked
                @param channel the channel to be deleted

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

Brain* Agent::getBrain()
{
    return m_brain;
}

Channel* Agent::getColor()
{
    return m_color;
}

quint32 Agent::getId() {
    return m_id;
}

Channel* Agent::getInputChannel(QString name)
{
    if(this->inputChannelExists(name))
        return(m_inputs.value(name));
    else
        return 0;
}

Channel* Agent::getOutputChannel(QString name)
{
    if(this->outputChannelExists(name))
        return(m_outputs.value(name));
    else
        return 0;
}

QVector3D* Agent::getPosition()
{
    return &m_position;
}

QVector3D* Agent::getRotation()
{
    return &m_rotation;
}

Scene* Agent::getScene()
{
    return m_scene;
}

bool Agent::inputChannelExists(QString name)
{
    if(m_inputs.count(name)>0)
        return true;
    else
        return false;
}

bool Agent::outputChannelExists(QString name)
{
    if(m_outputs.count(name)>0)
        return true;
    else
        return false;
}

void Agent::renderGL()
{
    m_body->renderGL();

//    glPushMatrix();
//    glLineWidth( 3.0 );
//    glColor3f( 0.5, 0.1, 1);
//    glutSolidSphere(10,20,10);
//    glPopMatrix();
}

void Agent::reset()
{
    m_position.setX(m_restPosition.x());
    m_position.setY(m_restPosition.y());
    m_position.setZ(m_restPosition.z());

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
