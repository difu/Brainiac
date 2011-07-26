#include "agent.h"
#include "brain/brain.h"
#include "brain/fuzzybase.h"
#include "brain/timer.h"
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
    m_scene=agent->getScene();
    m_body=new Body(this,agent->getBody());
    m_brain=new Brain(this,agent->getBrain());
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
        qreal loudAmplitude=otherAgent->getOutputChannel("sound.a")->getValue();
        qreal reception=loudAmplitude-distance;
        if(reception>loudestReception) {
            loudestReception=reception;
            loudestAgent=otherAgent;
            loudestAgentPosition=loudAgentPosition;
            loudestAmplitude=loudAmplitude;
        }
        // End sound stuff

    }

    // Postprocess sound stuff
    if(loudestAgent) { // has heard something
        // calculate angle about y axis
        QVector3D distVect=m_position-loudestAgentPosition;
        qreal scalar=QVector3D::dotProduct(distVect,QVector3D(1.0f,0.0f,0.0f));

        //qDebug() << "loudest agent from agent " << m_id << "is " << loudestAgent->getId() << loudestAgentPosition;
        qreal yRotation=m_rotation.y(); // we need to normalize that value, so store it in a temp var for norm function
        BrainiacGlobals::normalizeAngle(&yRotation);
        if(yRotation<0)
            yRotation=-yRotation;
        qreal angle=acos(scalar/(distVect.length())) * ((double)180.0)/BrainiacGlobals::PI + yRotation-270.0f;
        // determine, if the loudest agent is on this agent´s right or left side
        // place to points with distance 1 to current position on 90° and 270° and find out which point is closer
        QVector3D leftPoint;
        QVector3D rightPoint;
        leftPoint.setX(m_position.x()+BrainiacGlobals::sinGrad(m_rotation.y()+90.0f));
        leftPoint.setZ(m_position.z()+BrainiacGlobals::cosGrad(m_rotation.y()+90.0f));
        leftPoint.setY(0.0f);

        rightPoint.setX(m_position.x()+BrainiacGlobals::sinGrad(m_rotation.y()+270.0f));
        rightPoint.setZ(m_position.z()+BrainiacGlobals::cosGrad(m_rotation.y()+270.0f));
        rightPoint.setY(0.0f);
        //qDebug() << "Position:" << m_position << "left:" << leftPoint << "right:" << rightPoint;

        QVector3D leftDistVec=loudestAgentPosition-leftPoint;
        QVector3D rightDistVec=loudestAgentPosition-rightPoint;
//        if(leftDistVec.length()>rightDistVec.length()) {
//            //angle=-angle;
//            qDebug() << "Loudest Agent is to the right";
//        } else {
//            qDebug() << "Loudest Agent is to the left";
//        }
        if(angle>180.0f) {
            angle=-360.0f+angle;
        }
        BrainiacGlobals::normalizeAngle(&angle);
        //if(angle>180.0f) angle=-angle+270.0f;
//        qDebug() << "Angle of LA:"<< angle << "dist" << distVect.length() << m_position << "y rotation" << yRotation;
        m_iSoundX->setValue(angle);
        m_iSoundD->setValue(loudestReception/loudestAmplitude);
        m_iSoundF->setValue(loudestAgent->getOutputChannel("sound.f")->getValue());

    } else { // this agent hasn´t heard any sound
        m_iSoundX->setValue(0.0f);
        m_iSoundD->setValue(0.0f);
        m_iSoundF->setValue(0.0f);
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
        }
    }
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

    m_iSoundF=new Channel(0,10,0);
    addInputChannel(m_iSoundF,"sound.f");

    m_iSoundD=new Channel(0,1,0);
    addInputChannel(m_iSoundD,"sound.d");

}

void Agent::deleteConnection(quint32 parentId, quint32 childId)
{
    m_brain->disconnectFuzzies(childId,parentId);
}

void Agent::deleteChannel(Channel *channel)
{
    m_inputs.remove(m_inputs.key(channel));
    m_outputs.remove(m_inputs.key(channel));
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

Channel* Agent::getInputChannel(const QString &name) const
{
    if(this->inputChannelExists(name))
        return(m_inputs.value(name));
    else
        return 0;
}

Channel* Agent::getOutputChannel(const QString &name)
{
    if(this->outputChannelExists(name))
        return(m_outputs.value(name));
    else
        return 0;
}

const QVector3D* Agent::getPosition() const
{
    return &m_position;
}

const QVector3D* Agent::getRotation() const
{
    return &m_rotation;
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
    if(m_inputs.count(name)>0)
        return true;
    else
        return false;
}

bool Agent::outputChannelExists(const QString &name) const
{
    if(m_outputs.count(name)>0)
        return true;
    else
        return false;
}

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
