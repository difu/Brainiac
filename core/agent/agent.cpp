#include "agent.h"
#include "brain/brain.h"
#include "brain/fuzzybase.h"
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
    m_renderSoundEmission=true;
}

Agent::Agent(Agent *agent, quint32 id)  :
    QObject()
{
    m_id=id;
    createChannels();
    m_body=new Body(this,agent->getBody());
    m_brain=new Brain(this,agent->getBrain());
    m_scene=agent->getScene();
    m_renderSoundEmission=true;
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

void Agent::addConnection(quint32 childId, quint32 parentId, bool inverted)
{
    m_brain->connectFuzzies(childId, parentId,inverted);
}

void Agent::addOutputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max)
{
    m_brain->addOutputFuzz(id, name, channel, min,  max);
}

void Agent::addInputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max)
{
    m_brain->addInputFuzz(id, name, channel,  min,  max);
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

    Agent *loudestAgent=0;
    qreal loudestReception=0;
    qreal loudestAmplitude=0;
    QVector3D loudestAgentPosition;
    foreach(Agent *otherAgent,m_scene->getAgents()) {
        if(otherAgent==this) {
            //qDebug() << "found myself"  << agent->getId() << m_id;
            continue;
        }

        // Sound Stuff
        QVector3D loudAgentPosition=QVector3D(otherAgent->getPosition()->x(),otherAgent->getPosition()->y(),otherAgent->getPosition()->z());
        qreal distance=(loudAgentPosition-m_position).length();
        //qDebug() << "Distcance from agent " << m_id << "to " << agent->getId() << " is" << distance;
        loudestAmplitude=otherAgent->getOutputChannel("sound.a")->getValue();
        qreal reception=loudestAmplitude-distance;
        if(reception>loudestReception) {
            loudestReception=reception;
            loudestAgent=otherAgent;
            loudestAgentPosition=loudAgentPosition;
        }
        // End sound stuff

    }

    // Postprocess sound stuff
    if(loudestAgent) { // has heard something
        // calculate angle about y axis
        QVector3D distVect=m_position-loudestAgentPosition;
        qreal scalar=QVector3D::dotProduct(distVect,QVector3D(1.0f,0.0f,0.0f));

        //qDebug() << "loudest agent from agent " << m_id << "is " << loudestAgent->getId() << loudestAgentPosition;
        qreal angle=acos(scalar/(distVect.length())) * ((double)180.0)/BrainiacGlobals::PI-90.0f;

        BrainiacGlobals::normalizeAngle(&angle);
        //if(angle>180.0f) angle=-angle+270.0f;
        //qDebug() << "Angle of LA:"<< angle << "dist" << distVect.length() << m_position << "Dist Vector" << distVect << scalar << distVect.length()/loudestReception;
        m_iSoundX->setValue(angle);
        m_iSoundD->setValue(loudestReception/loudestAmplitude);

    } else { // this agent hasn´t heard any sound
        m_iSoundX->setValue(0.0f);
        m_iSoundD->setValue(0.0f);
    }
    //

    foreach(FuzzyBase *fuzz, m_brain->getFuzzies()) {
        if(fuzz->getType()==FuzzyBase::NOISE) { // noise nodes depend only on frame imformation
            fuzz->calculate();
        }
        if(fuzz->getType()==FuzzyBase::INPUT  && !fuzz->hasParents()) { // input nodes with parents are triggered implicite by their parents
            fuzz->calculate();
        }
    }
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

    m_oSoundA=new Channel(0);
    addOutputChannel(m_oSoundA,"sound.a");

    m_oSoundF=new Channel(0,10,0);
    addOutputChannel(m_oSoundF,"sound.f");

    m_iSoundX=new Channel(-180,180,0);
    addInputChannel(m_iSoundX,"sound.x");
    m_iSoundD=new Channel(0,1,0);
    addInputChannel(m_iSoundD,"sound.d");

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

/** \brief returns the body
**/
Body* Agent::getBody()
{
    return m_body;
}

/** \brief returns the brain
**/
Brain* Agent::getBrain()
{
    return m_brain;
}

/** \brief returns the bodie´s color
        by default this value is inherited recursivly to all segments of the body
        @sa Segment::setColorInherited(bool inherited)
**/
Channel* Agent::getColor()
{
    return m_color;
}

/** \brief returns the id of this agent
**/
quint32 Agent::getId() {
    return m_id;
}

/** \brief returns input channel
    @param name the name of the input channel
    @returns 0 if channel does not exist
**/
Channel* Agent::getInputChannel(QString name)
{
    if(this->inputChannelExists(name))
        return(m_inputs.value(name));
    else
        return 0;
}

/** \brief returns output channel
    @param name the name of the output channel
    @returns 0 if channel does not exist
**/
Channel* Agent::getOutputChannel(QString name)
{
    if(this->outputChannelExists(name))
        return(m_outputs.value(name));
    else
        return 0;
}

/** \brief returns the position of the agent in world space
**/
QVector3D* Agent::getPosition()
{
    return &m_position;
}

/** \brief returns the rotation of the agent in world space
**/
QVector3D* Agent::getRotation()
{
    return &m_rotation;
}

/** \brief true if sound emmisions should be rendered
**/
bool Agent::getRenderSoundEmission()
{
    return m_renderSoundEmission;
}

/** \brief returns the scene
    @sa Scene
**/
Scene* Agent::getScene()
{
    return m_scene;
}

/** \brief @returns true if channel exists
**/
bool Agent::inputChannelExists(QString name)
{
    if(m_inputs.count(name)>0)
        return true;
    else
        return false;
}

/** \brief @returns true if channel exists
**/
bool Agent::outputChannelExists(QString name)
{
    if(m_outputs.count(name)>0)
        return true;
    else
        return false;
}

/** \brief renders the agent in gl context
        calls the bodie´s render function
        it also draws sound emmissions
        @sa getRenderSoundEmission()
        @sa renderSoundEmission()
        @sa Body::renderGL()
**/
void Agent::renderGL()
{
    m_body->renderGL();
    // Draw sound emission
    if(m_renderSoundEmission) {
        glPushMatrix();
        glTranslated(getPosition()->x(),getPosition()->y(),getPosition()->z());
        glEnable(GL_DEPTH_TEST);
        glBegin(GL_LINE_LOOP);
            glRotated(getRotation()->x(),1,0,0);
            glRotated(getRotation()->y(),0,1,0);
            glRotated(getRotation()->z(),0,0,1);
            QColor col=BrainiacGlobals::getColorFromBrainiacColorValue(m_oSoundF->getValue()/m_oSoundF->getRange());
            glColor3f(col.redF(),col.greenF(),col.blueF());
            for ( float angle = 0; angle <= 2*BrainiacGlobals::PI; angle+=BrainiacGlobals::PI/30) {
                glVertex3f(m_oSoundA->getValue() * sin (angle), 5 , m_oSoundA->getValue() * cos (angle));
                glVertex3f(m_oSoundA->getValue() * sin (angle + BrainiacGlobals::PI/30) , 5 , m_oSoundA->getValue() * cos (angle + BrainiacGlobals::PI/30));
            }
        glEnd();
        glPopMatrix();
    } else {
        qDebug() << "not rendered";
    }
}

void Agent::renderSoundEmission(bool render)
{
    m_renderSoundEmission=render;
}

void Agent::reset()
{
    m_position.setX(m_restPosition.x());
    m_position.setY(m_restPosition.y());
    m_position.setZ(m_restPosition.z());

    m_rotation.setX(m_restRotation.x());
    m_rotation.setY(m_restRotation.y());
    m_rotation.setZ(m_restRotation.z());
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
