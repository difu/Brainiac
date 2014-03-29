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


#include <iostream>
#include "agent.h"
#include "brain/brain.h"
#include "brain/fuzzybase.h"
#include "brain/timer.h"
#include "body/body.h"
#include "core/brainiacglobals.h"
#include "channel.h"
#include "core/scene.h"
#include "core/agent/agentmanager.h"
#include "core/agent/body/animation/motiontreemanager.h"
#include "core/agent/body/animation/animationplayer.h"
#include "core/generator/locator.h"
#include "core/brainiaclogger.h"
#include <QDebug>
#include <QtGlobal>

Agent::Agent(AgentManager *manager, quint32 id) :
    QObject()
{
    m_id=id;
    m_agentManager=manager;
    m_scene=manager->getScene();
    createChannels();
    m_body=new Body(this);
    m_brain=new Brain(this,0);
    m_renderSoundEmission=true;
    m_locator=0;
}

Agent::Agent(Agent *otherAgent, quint32 id)  :
    QObject()
{
    m_id=id;
    createChannels();
    m_agentManager=otherAgent->getAgentManager();
    m_scene=otherAgent->getScene();
    m_body=new Body(this);
    m_body->setAnimations(otherAgent->getBody()->getAnimations());
    m_brain=new Brain(this,otherAgent->getBrain());
    m_renderSoundEmission=true;
    m_locator=0;
}

Agent::~Agent() {
    delete m_body;
    delete m_brain;
    foreach(Channel *chan,m_inputs) {
        chan->deleteLater();
    }
    m_inputs.clear();
    foreach(Channel *chan,m_outputs) {
        chan->deleteLater();
    }
    m_outputs.clear();
}

bool Agent::addInputChannel(Channel *channel, const QString &name)
{
    if(m_inputs.contains(name)) {
        qDebug() << __PRETTY_FUNCTION__ << "Channel " << name << "already exists!";
        return false;
    } else {
        m_inputs.insert(name,channel);
        return true;
    }
}

bool Agent::addOutputChannel(Channel *channel, const QString &name)
{
    if(m_outputs.contains(name)) {
        qDebug() << __PRETTY_FUNCTION__ << "Channel " << name << "already exists!";
        return false;
    } else {
        m_outputs.insert(name,channel);
        return true;
    }
}

void Agent::addConnection(quint32 childId, quint32 parentId, bool inverted)
{
    m_brain->connectFuzzies(childId, parentId,inverted);
}

void Agent::addAndFuzz(quint32 id, const QString &name, FuzzyAnd::Mode mode)
{
    m_brain->addAndFuzz(id, name, mode);
}

void Agent::addDefuzz(quint32 id, const QString &name, qreal defuzzValue, bool isElse)
{
    m_brain->addDefuzz(id,name,defuzzValue,isElse);
}

void Agent::addFuzzFuzz(quint32 id, const QString &name, FuzzyFuzz::Mode mode, FuzzyFuzz::InterpolationMode intMode, qreal p1, qreal p2, qreal p3, qreal p4)
{
    m_brain->addFuzzFuzz(id,name,mode,intMode,p1,p2,p3,p4);
}

void Agent::addOrFuzz(quint32 id, const QString &name, FuzzyOr::Mode mode)
{
    m_brain->addOrFuzz(id, name, mode);
}

void Agent::addOutputFuzz(quint32 id, const QString &name, const QString &channel, qreal min, qreal max)
{
    m_brain->addOutputFuzz(id, name, channel, min,  max);
}

void Agent::addInputFuzz(quint32 id, const QString &name, const QString &channel, qreal min, qreal max)
{
    m_brain->addInputFuzz(id, name, channel,  min,  max);
}

void Agent::addNoiseFuzz(quint32 id, const QString &name, qreal rate)
{
    m_brain->addNoiseFuzz(id, name,rate);
}

void Agent::addTimerFuzz(quint32 id, const QString &name, qreal rate, Timer::TimerMode mode)
{
    m_brain->addTimerFuzz(id, name, rate, mode);
}

void Agent::advance(AdvanceMode mode)
{
    //qDebug() << __PRETTY_FUNCTION__<< "Agent " << m_id;
    // Process movement
    m_newRotation.setX(m_rotation.x()+ m_rx->getValue());
    m_newRotation.setY(m_rotation.y()+ m_ry->getValue());
    m_newRotation.setZ(m_rotation.z()+ m_rz->getValue());

    m_newPosition.setX(m_position.x()+m_tz->getValue()*BrainiacGlobals::sinGrad(m_newRotation.y())+m_tx->getValue()*BrainiacGlobals::cosGrad(m_newRotation.y()));
    m_newPosition.setY(m_position.y()); //!< @todo Implement this!
    m_newPosition.setZ(m_position.z()+m_tz->getValue()*BrainiacGlobals::cosGrad(m_newRotation.y())+m_tx->getValue()*BrainiacGlobals::sinGrad(m_newRotation.y()));

    if(mode&Agent::BRAIN) {
        Agent *loudestAgent=0;
        qreal loudestReception=0;
        //qreal loudestAmplitude=0;
        QVector3D loudestAgentPosition;
        foreach(Agent *otherAgent,m_scene->getAgents()) {
            if(otherAgent==this) {
                //qDebug() << "found myself"  << agent->getId() << m_id;
                continue;
            }

            // Sound Stuff
            QVector3D loudAgentPosition=QVector3D(otherAgent->getPosition().x(),otherAgent->getPosition().y(),otherAgent->getPosition().z());
            //        qreal distance=(loudAgentPosition-m_position).length();
            //        //qDebug() << "Distcance from agent " << m_id << "to " << agent->getId() << " is" << distance;
            //        qreal loudAmplitude=otherAgent->getOutputChannel(BrainiacGlobals::ChannelName_Sound_a)->getValue();
            qreal reception=getOtherAgentSoundReception(otherAgent);
            if(reception>loudestReception) {
                loudestReception=reception;
                loudestAgent=otherAgent;
                loudestAgentPosition=loudAgentPosition;
                //loudestAmplitude=loudAmplitude;
            }
            // End sound stuff

        }

        // Postprocess sound stuff
        if(loudestAgent) { // has heard something
            // calculate angle about y axis
            qreal angle=this->getOtherAgentRelativeAngle(loudestAgent);
            //        qDebug() << "Angle of LA:"<< angle << "dist" << distVect.length() << m_position << "y rotation" << yRotation;

            m_iSoundX->setValue(angle);
            m_iSoundD->setValue(loudestReception/loudestAgent->getOutputChannel(BrainiacGlobals::ChannelName_Sound_a)->getOldValue() );
            m_iSoundF->setValue(loudestAgent->getOutputChannel(BrainiacGlobals::ChannelName_Sound_f)->getOldValue());
            m_iSoundOX->setValue(getOtherAgentRelativeOrientation(loudestAgent));
        } else { // this agent hasn´t heard any sound
            m_iSoundX->setValue(0.0);
            m_iSoundD->setValue(0.0);
            m_iSoundF->setValue(0.0);
            m_iSoundOX->setValue(0.0);
        }
        //

        foreach(FuzzyBase *fuzz, m_brain->getFuzzies()) {
            if(fuzz->getType()==FuzzyBase::NOISE) { // noise nodes depend only on frame information
                fuzz->calculate();
            } else if(fuzz->getType()==FuzzyBase::INPUT  && !fuzz->hasParents()) { // input nodes with parents are triggered implicite by their parents
                fuzz->calculate();
            } else if(fuzz->getType()==FuzzyBase::TIMER) {
                Timer *timer=(Timer *)fuzz;
                timer->advance();
            } else if(fuzz->getType()==FuzzyBase::DEFUZZ ) { // input nodes with parents are triggered implicite by their parents
                fuzz->calculate();
            } else if(fuzz->getType()==FuzzyBase::AND ) {
                FuzzyAnd *fuzzyAnd=(FuzzyAnd *)fuzz;
                if(fuzzyAnd->isSoundRule()) {
                    fuzzyAnd->calculateSound();
                }
            }
        }
        m_body->applyPlayers();


    }
    m_body->advance();
}

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

    m_body->updatePosition();
    //qDebug() << __PRETTY_FUNCTION__ << m_position;
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

    m_color=new Channel(0.0,1.0,0.0);
    addInputChannel(m_color,"color");
    addOutputChannel(m_color,"color");

    m_oSoundA=new Channel(0.0);
    addOutputChannel(m_oSoundA,BrainiacGlobals::ChannelName_Sound_a);

    m_oSoundF=new Channel(0.0,10.0,0.0);
    addOutputChannel(m_oSoundF,BrainiacGlobals::ChannelName_Sound_f);

    m_iSoundX=new Channel(-180.0,180.0,0.0);
    addInputChannel(m_iSoundX,BrainiacGlobals::ChannelName_Sound_x);

    m_iSoundOX=new Channel(-180.0,180.0,0.0);
    addInputChannel(m_iSoundOX,BrainiacGlobals::ChannelName_Sound_ox);

    m_iSoundF=new Channel(0.0,10.0,0.0);
    addInputChannel(m_iSoundF,BrainiacGlobals::ChannelName_Sound_f);

    m_iSoundD=new Channel(0.0,1.0,0.0);
    addInputChannel(m_iSoundD,BrainiacGlobals::ChannelName_Sound_d);

}

void Agent::deleteConnection(quint32 parentId, quint32 childId)
{
    m_brain->disconnectFuzzies(childId,parentId);
}

void Agent::deleteChannel(Channel *channel)
{
    QString name = m_inputs.key(channel);
    if(m_inputs.contains(name))
        m_inputs.remove(name);
    name = m_outputs.key(channel);
    if(m_outputs.contains(name))
        m_outputs.remove(name);
    delete channel;
}

void Agent::deleteFuzz(quint32 fuzzId)
{
    m_brain->deleteFuzz(fuzzId);
}

Body* Agent::getBody() const
{
    return m_body;
}

Brain* Agent::getBrain() const
{
    return m_brain;
}


Channel* Agent::getColor() const
{
    return m_color;
}

quint32 Agent::getId() const
{
    return m_id;
}

Channel* Agent::getInputChannel(const QString &name, Channel::ChannelNotExistOptions options) const
{
    if(this->inputChannelExists(name))
        return(m_inputs.value(name));
    else {
        switch(options) {
        case Channel::NONE:
            break;
        case Channel::NONE_WARN:
            qCWarning(bAgent) << __PRETTY_FUNCTION__ << "Input Channel "<< name << " does not exist!";
            break;
        case Channel::CREATE_IF_NOT_EXISTS:
            qCWarning(bAgent)  << __PRETTY_FUNCTION__ << "Input Channel "<< name << " cannot be created here!";
        }
    }
    return 0;
}

Channel* Agent::getOutputChannel(const QString &name, Channel::ChannelNotExistOptions options) const
{
    if(this->outputChannelExists(name))
        return(m_outputs.value(name));
    else {
        switch(options) {
        case Channel::NONE:
            break;
        case Channel::NONE_WARN:
            qCWarning(bAgent) << __PRETTY_FUNCTION__ << "Output Channel "<< name << " does not exist!";
            break;
        case Channel::CREATE_IF_NOT_EXISTS:
            qCWarning(bAgent)  << __PRETTY_FUNCTION__ << "Output Channel "<< name << " cannot be created here!";
        }
    }
    return 0;
}

qreal Agent::getOtherAgentRelativeAngle(const Agent &otherAgent) const
{
    QVector3D distVect=this->getOtherAgentRelativePosition(otherAgent);
    qreal scalar=-QVector3D::dotProduct(distVect,QVector3D(1.0,0.0,0.0));

    //qDebug() << "loudest agent from agent " << m_id << "is " << loudestAgent->getId() << loudestAgentPosition;

    // we need to normalize that value, so store it in a temp var for norm function
    qreal yRotation=-m_rotation.y()+90.0; // +90 because of z is forward axis
    BrainiacGlobals::normalizeAngle(&yRotation);
    if(yRotation<0)
        yRotation=360.0+yRotation;

    // the absolute angle
    qreal angle=acos(scalar/(distVect.length())) * ((double)180.0)/BrainiacGlobals::PI;// + yRotation;
    if(m_position.z()<otherAgent.getPosition().z()) {
        angle=360.0-angle;
    }

    // determine, if the loudest agent is on this agent´s right or left side
    // place to points with distance 1 to current position on 90° and 270° and find out which point is closer
    QVector3D leftPoint;
    QVector3D rightPoint;
    leftPoint.setX(m_position.x()+BrainiacGlobals::sinGrad(m_rotation.y()+90.0));
    leftPoint.setZ(m_position.z()+BrainiacGlobals::cosGrad(m_rotation.y()+90.0));
    leftPoint.setY(0.0);

    rightPoint.setX(m_position.x()+BrainiacGlobals::sinGrad(m_rotation.y()+270.0));
    rightPoint.setZ(m_position.z()+BrainiacGlobals::cosGrad(m_rotation.y()+270.0));
    rightPoint.setY(0.0);
    //qDebug() << "Position:" << m_position << "left:" << leftPoint << "right:" << rightPoint << yRotation;

    QVector3D otherPos(otherAgent.getPosition().x(),otherAgent.getPosition().y(),otherAgent.getPosition().z());

    //qDebug() << "Other Agent Pos" << otherPos;

    QVector3D leftDistVec=otherPos-leftPoint;
    QVector3D rightDistVec=otherPos-rightPoint;
    //qDebug() << "UNFILTEREDANGLE" << angle;

    qreal resAngle=0.0;

    if(leftDistVec.length()>rightDistVec.length()) {
        //qDebug() << "Loudest Agent is to the right";
        if(angle>180.0) {
            if(yRotation>angle) { // b>a
                resAngle=180+angle-yRotation;
            } else {
                resAngle=angle-180.0-yRotation;
            }
        } else {
            resAngle=180.0-(yRotation-angle);
        }
    } else {
        if(angle>180.0) {
            resAngle=yRotation-(angle-180.0);
        } else {
            if(yRotation<angle) {
                resAngle=180.0-(angle-yRotation);
            } else {
                resAngle=(yRotation-(angle)+180.0);
            }
        }
        resAngle=-resAngle;
        //qDebug() << "Loudest Agent is to the left";
    }
    BrainiacGlobals::normalizeAngle(&resAngle);
    return resAngle;
}

qreal Agent::getOtherAgentRelativeOrientation(const Agent &otherAgent) const
{
    qreal otherAgentOrientation=otherAgent.getRotation().y();
    qreal ownOrientation=m_rotation.y();
    BrainiacGlobals::normalizeAngle(&otherAgentOrientation);
    BrainiacGlobals::normalizeAngle(&ownOrientation);
    if(otherAgentOrientation<0.0) {
        otherAgentOrientation+=360.0;
    }
    if(ownOrientation<0.0) {
        ownOrientation+=360.0;
    }
    qreal diff=otherAgentOrientation-ownOrientation;

    qreal res=0;

    if(diff<-180.0) {
        res=diff+360.0;
    } else if (diff>180.0) {
        res=diff-360.0;
    } else {
        res=diff;
    }
    return -res;
}

qreal Agent::getOtherAgentSoundReception(const Agent &otherAgent) const
{
    qreal otherAgentAmplitude=otherAgent.getOutputChannel(BrainiacGlobals::ChannelName_Sound_a)->getOldValue();
    qreal distance=this->getOtherAgentRelativePosition(otherAgent).length();
    qreal reception=otherAgentAmplitude-distance;
    return qMax((qreal)0.0,reception);
}

QVector3D Agent::getOtherAgentRelativePosition(const Agent &otherAgent) const
{
    return QVector3D(otherAgent.getPosition().x(),otherAgent.getPosition().y(),otherAgent.getPosition().z())-m_position;
}

const QVector3D &Agent::getPosition() const
{
    return m_position;
}

const QVector3D &Agent::getRotation() const
{
    return m_rotation;
}

bool Agent::getRenderSoundEmission() const
{
    return m_renderSoundEmission;
}

Scene* Agent::getScene() const
{
    return m_scene;
}

bool Agent::inputChannelExists(const QString &name) const
{
    if(m_inputs.contains(name)) {
        return true;
    } else {
        return false;
    }
}

bool Agent::outputChannelExists(const QString &name) const
{
    if(m_outputs.contains(name)) {
        return true;
    } else {
        return false;
    }
}

void Agent::renderSoundEmission(bool render)
{
    m_renderSoundEmission=render;
}

void Agent::reset()
{
    if(m_locator) {
        m_position.setX(m_locator->getLocation().x());
        m_position.setY(m_locator->getLocation().y());
        m_position.setZ(m_locator->getLocation().z());

        m_rotation.setX(0.0);
        m_rotation.setY(m_locator->getLocation().w());
        m_rotation.setZ(0.0);
    } else {
        m_position=QVector3D();
        m_rotation=QVector3D();
    }

    //m_body->updatePosition();
    m_body->reset();
}

void Agent::setObjectName(const QString &name)
{
    QObject::setObjectName(name);
    m_body->getBodyRoot().get()->setName(name.toStdString());
}

void Agent::setLocator(Locator *locator)
{
    m_locator=locator;
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


void Agent::dDumpChannels()
{
    std::cout << "Agent:" << this->getId() << std::endl;
    std::cout << "Input Channels" << std::endl;
    QHashIterator<QString, Channel *> ii(m_inputs) ;
    while(ii.hasNext()) {
        ii.next();
        std::cout << ii.key().toStdString() << " Value " << ii.value()->getValue() << std::endl;
    }
    std::cout << "Output Channels" << std::endl;
    QHashIterator<QString, Channel *> io(m_outputs) ;
    while(io.hasNext()) {
        io.next();
        std::cout <<io.key().toStdString() << " Value " << io.value()->getValue() << std::endl;
    }
}
