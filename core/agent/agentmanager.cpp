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


#include "agentmanager.h"
#include "core/agent/agent.h"
#include "core/scene.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/fuzzydefuzz.h"
#include "core/agent/brain/fuzzyand.h"
#include "core/agent/brain/fuzzyor.h"
#include "core/agent/brain/fuzzybase.h"
#include "core/agent/brain/input.h"
#include "core/agent/brain/output.h"
#include "core/agent/brain/noise.h"
#include "core/agent/brain/fuzzyfuzz.h"
#include "core/agent/channel.h"
#include "core/group/group.h"
#include "core/agent/body/animation/animationcurve.h"
#include "core/agent/body/animation/animation.h"
#include "core/agent/body/bodymanager.h"
#include "core/agent/body/segment.h"
#include "core/agent/body/animation/motiontreemanager.h"
#include "core/agent/body/animation/motiontree.h"
#include "core/agent/body/animation/motiontreeaction.h"
#include "core/brainiacerror.h"
#include "gui/braineditor/braineditor.h"
#include "gui/bodyeditor/bodyeditor.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QVector3D>

AgentManager::AgentManager(Group *group)
{
    m_group=group;
    m_scene=m_group->getScene();
    m_id=0;
    m_bodyManager=new BodyManager(this);
    m_masterAgent=new Agent(this,0); // Id 0 is ok, its just a master agent
    m_masterAgent->getBody()->setAnimations(&m_animations);
    m_agents.append(m_masterAgent);
    m_spBodyAgent=cloneAgent(0);
    m_spActionAgent=cloneAgent(0);
    m_agents.append(m_spBodyAgent);
    m_agents.append(m_spActionAgent);
    m_brainEditor=new BrainEditor(m_scene,this);
    m_motionTreeManager=new MotionTreeManager(this);
}

void AgentManager::addSegmentFromConfig(QXmlStreamReader *reader, quint32 id, QString name, quint32 parent, quint32 editorX, quint32 editorY)
{
    QVector3D translation;
    QVector3D rotation;
    QVector3D restTranslation;
    QVector3D restRotation;
    QVector3D segSize;
    QList<BrainiacGlobals::RotTrans> rotTransOrder;
    qreal color;
    bool colorInherited;
    while(reader->readNextStartElement()) {
        if(reader->name()==BrainiacGlobals::XmlTranslationTag) {
            QXmlStreamAttributes attribs = reader->attributes();
            translation.setX(attribs.value("x").toString().toDouble());
            translation.setY(attribs.value("y").toString().toDouble());
            translation.setZ(attribs.value("z").toString().toDouble());
            reader->skipCurrentElement();
        } else if(reader->name()==BrainiacGlobals::XmlRotationTag) {
            QXmlStreamAttributes attribs = reader->attributes();
            rotation.setX(attribs.value("x").toString().toDouble());
            rotation.setY(attribs.value("y").toString().toDouble());
            rotation.setZ(attribs.value("z").toString().toDouble());
            reader->skipCurrentElement();
        } else if(reader->name()==BrainiacGlobals::XmlRestTranslationTag) {
            QXmlStreamAttributes attribs = reader->attributes();
            restTranslation.setX(attribs.value("x").toString().toDouble());
            restTranslation.setY(attribs.value("y").toString().toDouble());
            restTranslation.setZ(attribs.value("z").toString().toDouble());
            reader->skipCurrentElement();
        } else if(reader->name()==BrainiacGlobals::XmlRestRotationTag) {
            QXmlStreamAttributes attribs = reader->attributes();
            restRotation.setX(attribs.value("x").toString().toDouble());
            restRotation.setY(attribs.value("y").toString().toDouble());
            restRotation.setZ(attribs.value("z").toString().toDouble());
            reader->skipCurrentElement();
        } else if(reader->name()=="Color") {
            QXmlStreamAttributes attribs = reader->attributes();
            color=attribs.value("value").toString().toDouble();
            colorInherited=attribs.value("inherited").toString().compare("true",Qt::CaseInsensitive)==0;
            reader->skipCurrentElement();
        } else if(reader->name()==BrainiacGlobals::XmlSizeTag) {
            QXmlStreamAttributes attribs = reader->attributes();
            segSize.setX(attribs.value("x").toString().toDouble());
            segSize.setY(attribs.value("y").toString().toDouble());
            segSize.setZ(attribs.value("z").toString().toDouble());
            reader->skipCurrentElement();
        } else if(reader->name()==BrainiacGlobals::XmlRotTransOrderTag) {
            QXmlStreamAttributes attribs = reader->attributes();
            QStringRef orderRef=attribs.value("order");
            QString order(orderRef.toString());
            foreach(QString rt,order.split(" ")) {
                if(rt=="RX") {
                    rotTransOrder.append(BrainiacGlobals::RX);
                } else if(rt=="RY") {
                    rotTransOrder.append(BrainiacGlobals::RY);
                } else if(rt=="RZ") {
                    rotTransOrder.append(BrainiacGlobals::RZ);
                } else if(rt=="TX") {
                    rotTransOrder.append(BrainiacGlobals::TX);
                } else if(rt=="TY") {
                    rotTransOrder.append(BrainiacGlobals::TY);
                } else if(rt=="TZ") {
                    rotTransOrder.append(BrainiacGlobals::TZ);
                }
            }
            reader->skipCurrentElement();
        } else if(reader->name()==BrainiacGlobals::XmlSphereTag) {
            QXmlStreamAttributes attribs = reader->attributes();
            reader->skipCurrentElement();
            m_bodyManager->setNewSegmentType(BrainiacGlobals::SPHERESEGMENT);
        } else if(reader->name()==BrainiacGlobals::XmlBoxTag) {
            QXmlStreamAttributes attribs = reader->attributes();
            reader->skipCurrentElement();
            m_bodyManager->setNewSegmentType(BrainiacGlobals::BOXSEGMENT);
        } else if(reader->name()==BrainiacGlobals::XmlTubeTag) {
            QXmlStreamAttributes attribs = reader->attributes();
            reader->skipCurrentElement();
            m_bodyManager->setNewSegmentType(BrainiacGlobals::TUBESEGMENT);
        }
    }

    m_bodyManager->setNewSegmentId( id);
    m_bodyManager->setNewSegmentName(name);
    m_bodyManager->setNewSegmentParentId(parent);
    m_bodyManager->setNewSegmentRestRotation(restRotation);
    m_bodyManager->setNewSegmentRestTranslation(restTranslation);
    m_bodyManager->setNewSegmentRotation(rotation);
    m_bodyManager->setNewSegmentTranslation(translation);
    m_bodyManager->setNewSegmentRotationTranslationOrder(rotTransOrder);
    m_bodyManager->setNewSegmentSize(segSize);
    m_bodyManager->setNewSegmentColor(color);
    m_bodyManager->setNewSegmentColorInherited(colorInherited);
    setBodyEditorTranslation(id,editorX,editorY);
    m_bodyManager->createNewSegment();


    //m_masterAgent->getBody()->addSkeletonNode(newNode,parent);

}

quint32 AgentManager::addAndFuzz(quint32 editorX, quint32 editorY)
{
    return addAndFuzz(QString("And"),BrainiacGlobals::FuzzAndModeMin,editorX,editorY);
}

quint32 AgentManager::addAndFuzz(QString name, QString mode, quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addAndFuzz(id,name,mode,editorX,editorY);
    return id;
}

void AgentManager::addAndFuzz(quint32 id, QString name, QString mode, quint32 editorX, quint32 editorY)
{
    FuzzyAnd::Mode andMode;
    if(QString::compare(BrainiacGlobals::FuzzAndModeMin,mode,Qt::CaseInsensitive)==0) {
        andMode=FuzzyAnd::MIN;
    } else {
        andMode=FuzzyAnd::PRODUCT;
    }
    //m_masterAgent->addAndFuzz(id,name,andMode);
    foreach(Agent* agent,m_agents) {
        agent->addAndFuzz(id,name,andMode);
    }
    m_brainIdGenerator.registerId(id);
    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
    m_brainEditor->addFuzzyItem(id);
}

quint32 AgentManager::addFuzzFuzz(quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addFuzzFuzz(id,"Fuzz",BrainiacGlobals::FuzzFuzzModeTrapezoid,BrainiacGlobals::FuzzFuzzInterpolationSine,0.2f,0.4f,0.6f,0.8f,editorX,editorY);
    return id;
}

quint32 AgentManager::addFuzzFuzz(QString name, QString mode, QString intMode, qreal p1, qreal p2, qreal p3, qreal p4, quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addFuzzFuzz(id,name,mode,intMode,p1,p2,p3,p4,editorX,editorY);
    return id;
}

void AgentManager::addFuzzFuzz(quint32 id, QString name, QString mode, QString intMode, qreal p1, qreal p2, qreal p3, qreal p4, quint32 editorX, quint32 editorY)
{
    FuzzyFuzz::Mode fuzzMode=FuzzyFuzz::TRAPEZOID;;
    FuzzyFuzz::InterpolationMode interMode=FuzzyFuzz::LINEAR;
    if(QString::compare(BrainiacGlobals::FuzzFuzzModeTrapezoid,mode,Qt::CaseInsensitive )==0) {
        fuzzMode=FuzzyFuzz::TRAPEZOID;
    } else if(QString::compare("triangle",mode,Qt::CaseInsensitive )==0) {
        fuzzMode=FuzzyFuzz::TRIANGLE;
    } else if(QString::compare("dirac",mode,Qt::CaseInsensitive )==0) {
        fuzzMode=FuzzyFuzz::DIRAC;
    } else if(QString::compare("activate",mode,Qt::CaseInsensitive )==0) {
        fuzzMode=FuzzyFuzz::ACTIVATE;
    } else if(QString::compare("deactivate",mode,Qt::CaseInsensitive )==0) {
        fuzzMode=FuzzyFuzz::DEACTIVATE;
    } else {
        qDebug() << __PRETTY_FUNCTION__ << "unkown mode "<< mode << ", setting to trapezoid";
    }

    if(QString::compare("linear",intMode,Qt::CaseInsensitive )==0) {
        interMode=FuzzyFuzz::LINEAR;
    } else if(QString::compare(BrainiacGlobals::FuzzFuzzInterpolationSine,intMode,Qt::CaseInsensitive )==0) {
        interMode=FuzzyFuzz::SINE;
    } else if(QString::compare("quad",intMode,Qt::CaseInsensitive )==0) {
        interMode=FuzzyFuzz::QUAD;
    }

    //m_masterAgent->addFuzzFuzz(id,name,fuzzMode,interMode,p1,p2,p3,p4);
    foreach(Agent* agent,m_agents) {
        agent->addFuzzFuzz(id,name,fuzzMode,interMode,p1,p2,p3,p4);
    }
    m_brainIdGenerator.registerId(id);
    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
    m_brainEditor->addFuzzyItem(id);
}

quint32 AgentManager::addDefuzz(quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addDefuzz(id,"Defuzz",1.0,false,editorX,editorY);
    return id;
}

quint32 AgentManager::addDefuzz(QString name, qreal defuzzValue, bool isElse, quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addDefuzz(id,name,defuzzValue,isElse,editorX,editorY);
    return id;
}

void AgentManager::addDefuzz(quint32 id, QString name, qreal defuzzValue, bool isElse, quint32 editorX, quint32 editorY)
{
    //m_masterAgent->addDefuzz(id,name,defuzzValue,isElse);
    foreach(Agent* agent,m_agents) {
        agent->addDefuzz(id,name,defuzzValue,isElse);
    }
    m_brainIdGenerator.registerId(id);
    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
    m_brainEditor->addFuzzyItem(id);
}

quint32 AgentManager::addOrFuzz(quint32 editorX, quint32 editorY)
{
    return addOrFuzz("Or",BrainiacGlobals::FuzzOrModeMax,editorX,editorY);
}

quint32 AgentManager::addOrFuzz(QString name, QString mode, quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addOrFuzz(id,name,mode,editorX,editorY);
    return id;
}

void AgentManager::addOrFuzz(quint32 id, QString name, QString mode, quint32 editorX, quint32 editorY)
{
    FuzzyOr::Mode orMode;
    if(QString::compare(BrainiacGlobals::FuzzOrModeMax,mode,Qt::CaseInsensitive)==0) {
        orMode=FuzzyOr::MAX;
    } else {
        orMode=FuzzyOr::SUM;
    }
    //m_masterAgent->addOrFuzz(id,name,orMode);
    foreach(Agent* agent,m_agents) {
        agent->addOrFuzz(id,name,orMode);
    }
    m_brainIdGenerator.registerId(id);
    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
    m_brainEditor->addFuzzyItem(id);
}

quint32 AgentManager::addOutputFuzz(quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addOutputFuzz(id,"Output","",0.0f,1.0f,editorX,editorY);
    return id;
}

quint32 AgentManager::addOutputFuzz(QString name, QString channel, qreal min, qreal max, quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addOutputFuzz(id,name,channel,min,max,editorX,editorY);
    return id;
}

void AgentManager::addOutputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max, quint32 editorX, quint32 editorY)
{
    //m_masterAgent->addOutputFuzz(id, name, channel, min, max);
    //Output *out=(Output*)m_masterAgent->getBrain()->getFuzzy(id);
    foreach(Agent* agent,m_agents) {
        agent->addOutputFuzz(id, name, channel, min, max);
        //Output *out=(Output*)agent->getBrain()->getFuzzy(id);
    }
    m_brainIdGenerator.registerId(id);
    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
    m_brainEditor->addFuzzyItem(id);
}

quint32 AgentManager::addInputFuzz(quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addInputFuzz(id,"Input","",0.0f,1.0f,editorX,editorY);
    return id;
}

quint32 AgentManager::addInputFuzz(QString name, QString channel, qreal min, qreal max, quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addInputFuzz(id,name,channel,min,max,editorX,editorY);
    return id;
}

void AgentManager::addInputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max, quint32 editorX, quint32 editorY)
{
    //m_masterAgent->addInputFuzz(id, name, channel, min, max);
    foreach(Agent* agent,m_agents) {
        agent->addInputFuzz(id, name, channel, min, max);
    }
    setFuzzyChannelName(id,channel); //!< @bug @todo Channel name must be set here to determine if an input is a sound input node...
    m_brainIdGenerator.registerId(id);
    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
    m_brainEditor->addFuzzyItem(id);
}

quint32 AgentManager::addNoiseFuzz(quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addNoiseFuzz(id,"Noise",1.0f,editorX,editorY);
    return id;
}

quint32 AgentManager::addNoiseFuzz(QString name, qreal rate, quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addNoiseFuzz(id,name,rate,editorX,editorY);
    return id;
}

void AgentManager::addNoiseFuzz(quint32 id, QString name, qreal rate, quint32 editorX, quint32 editorY)
{
    m_masterAgent->addNoiseFuzz(id, name, rate);
    //Noise *noise=(Noise*) m_masterAgent->getBrain()->getFuzzy(id);
    foreach(Agent* agent,m_group->getAgents()) {
        agent->addNoiseFuzz(id, name, rate);
    }
    m_brainIdGenerator.registerId(id);
    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
    m_brainEditor->addFuzzyItem(id);
}

quint32 AgentManager::addTimerFuzz(quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addTimerFuzz(id,"Timer",1,BrainiacGlobals::FuzzTimerModeIfStopped,editorX,editorY);
    return id;
}

quint32 AgentManager::addTimerFuzz(QString name, qreal rate, QString mode, quint32 editorX, quint32 editorY)
{
    quint32 id=m_brainIdGenerator.getNewId();
    addTimerFuzz(id,name,rate,mode,editorX,editorY);
    return id;
}

void AgentManager::addTimerFuzz(quint32 id, QString name, qreal rate, QString mode, quint32 editorX, quint32 editorY)
{
    if(QString::compare(BrainiacGlobals::FuzzTimerModeIfStopped,mode,Qt::CaseInsensitive)==0) {
        //m_masterAgent->addTimerFuzz(id,name,rate,Timer::IFSTOPPED);
        foreach(Agent* agent,m_agents) {
            agent->addTimerFuzz(id,name,rate,Timer::IFSTOPPED);
        }
    }
    m_brainIdGenerator.registerId(id);
    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
    m_brainEditor->addFuzzyItem(id);
}

void AgentManager::addConnector(quint32 childId, quint32 parentId, bool inverted)
{
    //m_masterAgent->addConnection(childId, parentId, inverted);//!< @todo check, if connection already exists
    foreach(Agent* agent,m_agents) {
        agent->addConnection(childId, parentId, inverted);
    }
    updateSoundConfigs();
    m_brainEditor->addConnector(parentId,childId);
}

void AgentManager::addInputChannel(const QString &name, qreal min, qreal max)
{
    foreach(Agent* agent,m_agents) {
        Channel *c=new Channel(min,max);
        agent->addInputChannel(c,name);
    }
}

void AgentManager::addOutputChannel(const QString &name, qreal min, qreal max)
{
    foreach(Agent* agent,m_agents) {
        Channel *c=new Channel(min,max);
        agent->addOutputChannel(c,name);
    }
}

Agent* AgentManager::cloneAgent(quint32 id)
{
    Agent *agent=new Agent(m_masterAgent,id);
    m_bodyManager->applyBodyOnAgent(agent);
    return agent;
}

Agent* AgentManager::createNewAgentInstance(quint32 id)
{
    Agent *agent=cloneAgent(id);
    m_agents.append(agent);
    return agent;
}

void AgentManager::deleteOutputChannel(const QString &name)
{
    foreach(Agent* agent,m_agents) {
        //agent->deleteChannel();
    }
}

void AgentManager::deleteConnector(quint32 childId, quint32 parentId)
{
    //m_masterAgent->deleteConnection(parentId,childId);
    foreach(Agent* agent,m_agents) {
        agent->deleteConnection(parentId,childId);
    }
    updateSoundConfigs();
    m_brainEditor->deleteConnector(parentId,childId);
}

void AgentManager::deleteFuzz(quint32 fuzzId)
{
    //m_masterAgent->deleteFuzz(fuzzId);
    foreach(Agent* agent,m_agents) {
        agent->deleteFuzz(fuzzId);
    }
    updateSoundConfigs();
    m_brainEditor->deleteFuzzyItem(fuzzId);
}

QHash<quint32, QPoint> AgentManager::getEditorSegmentNodeLocations()
{
    return m_editorSegmentNodeLocations;
}

QHash<quint32, QPoint> AgentManager::getEditorFuzzyLocations()
{
    return m_editorFuzzyLocations;
}

FuzzyBase::LogicType AgentManager::getFuzzType(quint32 fuzzId)
{
    FuzzyBase *fuzz=m_masterAgent->getBrain()->getFuzzy(fuzzId);
    if(fuzz) {
        return fuzz->getType();
    }
    else return (FuzzyBase::LogicType) 0;
}

bool AgentManager::loadConfig()
{
    QFile file(m_fileName);
    if(file.open(QIODevice::ReadOnly)) {
        QXmlStreamReader reader;
        reader.setDevice(&file);
        while(reader.readNextStartElement()) {
            if(reader.name()=="Agentconfig") {
                while(reader.readNextStartElement()) {
                    if(reader.name()=="Agent") {
                        QXmlStreamAttributes attribs = reader.attributes();
                        setId(attribs.value("id").toString().toInt());
                        setName(attribs.value("name").toString());
                        while(reader.readNextStartElement()) {
                            if(reader.name()=="Body") {
                                while(reader.readNextStartElement()) {
                                    if(reader.name()=="Segment") {
                                        QXmlStreamAttributes attribs = reader.attributes();
//                                        if( QString::compare( attribs.value("type").toString(),QString("sphere"),Qt::CaseInsensitive ) == 0 ) {
//                                            addSphereFromConfig(&reader,attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("parent").toString().toInt());
//                                            //reader.skipCurrentElement();
//                                        }
                                        addSegmentFromConfig(&reader,attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("parent").toString().toInt(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());

                                    }else {
                                        reader.skipCurrentElement();
                                    }
                                }
                            } else if(reader.name()==BrainiacGlobals::XmlMotionTreesTag) {
                                m_motionTreeManager->loadConfig(&reader);
                            } else if(reader.name()=="Brain") {
                                while(reader.readNextStartElement()) {
                                    if(reader.name()=="Output") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addOutputFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("channel").toString(),attribs.value("min").toString().toDouble(),attribs.value("max").toString().toDouble(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Input") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addInputFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("channel").toString(),attribs.value("min").toString().toDouble(),attribs.value("max").toString().toDouble(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="And") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addAndFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value(BrainiacGlobals::XmlModeAttrib).toString(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Defuzz") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addDefuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("defuzzvalue").toString().toDouble(), QString::compare(attribs.value("iselse").toString(),QString("true"),Qt::CaseInsensitive )==0,attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Or") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addOrFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value(BrainiacGlobals::XmlModeAttrib).toString(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Noise") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addNoiseFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("rate").toString().toDouble(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Fuzz") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addFuzzFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value(BrainiacGlobals::XmlModeAttrib).toString(),attribs.value("interpolation").toString(),attribs.value("p1").toString().toDouble(),attribs.value("p2").toString().toDouble(),attribs.value("p3").toString().toDouble(),attribs.value("p4").toString().toDouble(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Timer") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addTimerFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("rate").toString().toDouble(),attribs.value(BrainiacGlobals::XmlModeAttrib).toString(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Connector") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addConnector(attribs.value("child").toString().toUInt(),attribs.value("parent").toString().toUInt(),attribs.value("inverted").toString().toUInt()!=0);
                                        reader.skipCurrentElement();
                                    }else {
                                        reader.skipCurrentElement();
                                    }
                                }
                            } else if(reader.name()=="Animations") {
                                while(reader.readNextStartElement()) {
                                    if(reader.name()=="Animation") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        QString animFileName=attribs.value(BrainiacGlobals::XmlFileNameAttrib).toString();
                                        QString animAbsoluteFileName=m_scene->getAbsoluteFileDir()%"/"%animFileName;
                                        Animation *anim=Animation::loadAnimation(animAbsoluteFileName);
                                        //QString name=attribs.value(BrainiacGlobals::XmlNameAttrib).toString();
//                                        if(id<=0) {
//                                            qWarning() <<__PRETTY_FUNCTION__ << "Wrong animation id";
//                                        }
                                        m_animations.insert(anim->name(),anim);
                                        //m_animationIdGenerator.registerId(id);
                                        reader.skipCurrentElement();
                                    }else {
                                        reader.skipCurrentElement();
                                    }
                                }
                            }else {
                                reader.skipCurrentElement();
                            }
                        }
                    } else {
                        reader.skipCurrentElement();
                    }
                }
            } else {
                reader.skipCurrentElement();
            }
        }
        //updateSoundConfigs();
        return true;
    }
    return false;
}

bool AgentManager::loadAnimation(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error while opening file " << filename;
        return false;
    }
    if(filename.endsWith(".bvh",Qt::CaseInsensitive)) {
        return loadAnmationBVH(file);
    } else if(filename.endsWith(".baf")) {
        Animation *anim=Animation::loadAnimation(filename);
        if(anim) {
            m_animations.insert(anim->name(),anim);
        } else {
            qWarning() << __PRETTY_FUNCTION__ << "Could not load "<< filename;
        }
        qDebug() << "anim";
    } else {
        qWarning() << __PRETTY_FUNCTION__ << "File format not supported " << filename;
    }
    return false;

}

bool AgentManager::loadAnmationBVH(QFile &file)
{
//    AnimationCurve *tst=new AnimationCurve();
//    tst->addKeyFrame(0,1.01f);
//    tst->addKeyFrame(1,2);
//    tst->addKeyFrame(2,4);
//    tst->addKeyFrame(3,8);
//    tst->dPrintKeyFrames();
//    qDebug() << tst->getValue(0.5f) << tst->getValue(1.5f) << tst->getValue(0) << tst->getValue(4);
    QString nodeName;
    QHash<QString, AnimationCurve*> animCurves;
    QList<AnimationCurve *> curveIndex;
    qreal timeDelta=0.0f;
    quint32 valuesLine=0;
    bool animationDataStarts=false;
    while (!file.atEnd()) {
        QByteArray line = file.readLine().trimmed();
        if(!animationDataStarts) {
            if(line.startsWith("ROOT") || line.startsWith("JOINT")) {
                nodeName=line.split(' ').last();
            } else if(line.startsWith("CHANNELS")) {
                // quint32 numChannels=line.split(' ').at(1).toInt();
                // qDebug() << "Node" << nodeName << "has " << numChannels <<"Channels";
                foreach(QByteArray channelItem,line.split(' ')) {
                    if(channelItem=="Xrotation") {
                        AnimationCurve *curve=new AnimationCurve();
                        animCurves.insert(nodeName%":rx",curve);
                        curveIndex.append(curve);
                    } else if(channelItem=="Yrotation") {
                        AnimationCurve *curve=new AnimationCurve();
                        animCurves.insert(nodeName%":ry",curve);
                        curveIndex.append(curve);
                    } else if(channelItem=="Zrotation") {
                        AnimationCurve *curve=new AnimationCurve();
                        animCurves.insert(nodeName%":rz",curve);
                        curveIndex.append(curve);
                    } else if(channelItem=="Xposition") {
                        AnimationCurve *curve=new AnimationCurve();
                        animCurves.insert(nodeName%":tx",curve);
                        curveIndex.append(curve);
                    } else if(channelItem=="Yposition") {
                        AnimationCurve *curve=new AnimationCurve();
                        animCurves.insert(nodeName%":ty",curve);
                        curveIndex.append(curve);
                    } else if(channelItem=="Zposition") {
                        AnimationCurve *curve=new AnimationCurve();
                        animCurves.insert(nodeName%":tz",curve);
                        curveIndex.append(curve);
                    }
                }
            } else if(line.startsWith("Frame Time")) {
                animationDataStarts=true;
                timeDelta=line.split(':').at(1).toDouble();
            }
        } else {// if(!animationDataStarts) {
            int index=0;
            foreach(QByteArray animValue,line.split(' ')) {
                curveIndex.at(index)->addKeyFrame((qreal)valuesLine*timeDelta,animValue.toDouble());
                index++;
            }
            valuesLine++;
        }
        //return true;
    }
    QFileInfo fInfo(file);
    Animation *anim=new Animation(animCurves,fInfo.fileName().split('.').first());
    m_animations.insert(anim->name(),anim);
//    foreach(Animation *anim,m_animations) {
//        qDebug() << anim->name();
//        QHashIterator<QString, AnimationCurve *> i(anim->curves()) ;
//        while(i.hasNext()) {
//            i.next();
//            qDebug() << i.key();
//            i.value()->dPrintKeyFrames(0,3);
//        }
//    }
    return true;
}

bool AgentManager::loadSkeleton(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error while opening file " << filename;
        return false;
    }
    QFileInfo fInfo(file);
    if(fInfo.completeSuffix().compare(QString("bvh"),Qt::CaseInsensitive)==0) {
        return loadSkeletonBVH(file);
    }
    return false;
}

bool AgentManager::loadSkeletonBVH( QFile &file)
{
    quint32 nodeId=0;
    QString nodeName;
    bool isEndSite=false;
    QList<quint32> nodeStack; // Pointer, QObjects must not have a copy constructor
    QHash<quint32, QVector3D> siteOffsets;
    while (!file.atEnd()) {
        QByteArray line = file.readLine().trimmed();
        if(line.startsWith("End Site")) {
            isEndSite=true;
            continue;
        }
        if(line.startsWith("ROOT") || line.startsWith("JOINT")) {
            nodeId++;
            nodeName=line.split(' ').last();
            Segment seg;
            seg.setName(nodeName);
            seg.setId(nodeId);
            seg.setType(BrainiacGlobals::TUBESEGMENT);

            if(nodeStack.empty()) {
                // we have the root node!
                seg.setParentId(0);
            } else {
                seg.setParentId(nodeStack.last());
            }

//            SkeletonNodeBox *box=new SkeletonNodeBox(nodeId,nodeName,m_masterAgent->getBody());
//            //SkeletonNode *box=new SkeletonNode(SkeletonNode::TUBE,nodeId,nodeName,m_masterAgent->getBody());
//            box->setColor(0.1f);
//            if(nodeStack.isEmpty()) {
//                m_masterAgent->getBody()->addSkeletonNode(box,0);
//            } else {
//                nodeStack.last()->addNode(box);
//            }
            setBodyEditorTranslation(nodeId,0,0);
            nodeStack.push_back(seg.getId());
            m_bodyManager->setNewSegmentBySegment(seg);

            bool success=m_bodyManager->createNewSegment(true);
            if(!success) {
                qDebug() << __PRETTY_FUNCTION__ << "Whoops";
            }
        } else if(line.startsWith("OFFSET")) {
            if(!isEndSite) {
                QVector3D translation;
                translation.setX(line.split(' ').at(1).toDouble());
                translation.setY(line.split(' ').at(2).toDouble());
                translation.setZ(line.split(' ').at(3).toDouble());
                if(nodeStack.empty()) {
                    //BrainiacGlobals::LAST_ERROR_TEXT=QString("Parse Error in BVH file");
                    BrainiacError::setLastError(BrainiacError::FILE_PARSER_FAILED,"",0,"BVH File corrupt. Offset tag, but nodestack is empty!");
                    qDebug() << __PRETTY_FUNCTION__ << "Parse error";
                    return false;
                } else {
                    //nodeStack.last()->setRestTranslation(translation);
                    m_bodyManager->setSegmentRestTranslation(nodeStack.last(),translation.x(),translation.y(),translation.z());
                }
            } else { // End Site
                if(!nodeStack.isEmpty()) {
                    QVector3D translation;
                    translation.setX(line.split(' ').at(1).toDouble());
                    translation.setY(line.split(' ').at(2).toDouble());
                    translation.setZ(line.split(' ').at(3).toDouble());
                    siteOffsets.insert(nodeStack.last(),translation);
                } else {
                    BrainiacError::setLastError(BrainiacError::FILE_PARSER_FAILED,"",0,"BVH File corrupt. Offset tag, but nodestack is empty!");
                    qDebug() << __PRETTY_FUNCTION__ << "Parse error";
                    return false;
                }
            }
        } else if(line.startsWith("}")) {
            if(isEndSite) {
                isEndSite=false;
                continue;
            } else if(!nodeStack.isEmpty()) {
                nodeStack.removeLast();
                if(false) {
                    qDebug() << __PRETTY_FUNCTION__ << "Parse error?!";
                    /**
                     * @todo delete nodeStack, body etc properly
                     */

                    return false;
                }
            } else {
                qWarning() << __PRETTY_FUNCTION__ << "error while parsing "<< line << "NodeId" << nodeName;
                return false;
            }
        } else if(line.startsWith("CHANNELS")) {
            QList<BrainiacGlobals::RotTrans> rt;
            foreach(QByteArray channelItem,line.split(' ')) {
                if(channelItem=="Xrotation")
                    rt.prepend(BrainiacGlobals::RX);
                else if(channelItem=="Yrotation")
                    rt.prepend(BrainiacGlobals::RY);
                else if(channelItem=="Zrotation")
                    rt.prepend(BrainiacGlobals::RZ);
                else if(channelItem=="Xposition")
                    rt.prepend(BrainiacGlobals::TX);
                else if(channelItem=="Yposition")
                    rt.prepend(BrainiacGlobals::TY);
                else if(channelItem=="Zposition")
                    rt.prepend(BrainiacGlobals::TZ);
            }
            setSegmentRotationTranslationOrder(nodeId,rt);
//            //qDebug() <<  __PRETTY_FUNCTION__ << "Rot Trans order" << nodeStack.last()->objectName() << "" << rt;
        }
        //qDebug() << __PRETTY_FUNCTION__  << line;
    }
    foreach(SegmentShape *seg,m_bodyManager->getSegments()) {
//        qDebug() << "Processing " << seg->getName() << seg->getId();
        QVector3D offset;
        foreach(quint32 childId,m_bodyManager->getSegmentChildIds(seg->getId()) ) {
            offset+=m_bodyManager->getSegment(childId).getRestTranslation();
        }
        m_bodyManager->setSegmentSize(seg->getId(),1,10,1);
        if(m_bodyManager->getSegmentChildIds(seg->getId()).count()>0) {
            offset/=m_bodyManager->getSegmentChildIds(seg->getId()).count()+1;
            m_bodyManager->setSegmentTranslation(seg->getId(),offset.x()/2,offset.y()/2,offset.z()/2);
            QVector3D diff=seg->getRestRotation()- offset;
            qreal length=diff.length(); //!< @bug @todo Does not work, rewrite
            qreal xrot=acos(diff.x()/length);
            qreal yrot=acos(diff.y()/length);
            qreal zrot=acos(diff.z()/length);
            m_bodyManager->setSegmentSize(seg->getId(),1,1,length);
            m_bodyManager->setSegmentRotation(seg->getId(),BrainiacGlobals::rad2grad(xrot),BrainiacGlobals::rad2grad(yrot)-90.0,BrainiacGlobals::rad2grad(zrot)+90.0); //! \todo 90 degrees offset configurable
//            qDebug() << "Rots:"  << BrainiacGlobals::rad2grad(xrot) << BrainiacGlobals::rad2grad(yrot) << BrainiacGlobals::rad2grad(zrot);

        } else {
            if(siteOffsets.contains(seg->getId())) {
                QVector3D siteOffset=siteOffsets.value(seg->getId());
                m_bodyManager->setSegmentTranslation(seg->getId(),siteOffset.x()/2,siteOffset.y()/2,siteOffset.z()/2);
                QVector3D diff=seg->getRestRotation()- siteOffset;
                qreal length=diff.length();
                qreal xrot=acos(diff.x()/length);
                qreal yrot=acos(diff.y()/length);
                qreal zrot=acos(diff.z()/length);
                m_bodyManager->setSegmentSize(seg->getId(),1,1,length);
                m_bodyManager->setSegmentRotation(seg->getId(),BrainiacGlobals::rad2grad(xrot),BrainiacGlobals::rad2grad(yrot),BrainiacGlobals::rad2grad(zrot));

                m_bodyManager->setSegmentTranslation(seg->getId(),siteOffset.x(),siteOffset.y(),siteOffset.z());
            } else {
                BrainiacError::setLastError(BrainiacError::FILE_PARSER_FAILED,"",0,"BVH File corrupt. Node End Site vector found");
                qDebug() << __PRETTY_FUNCTION__ << "Parse error";
                return false;

            }
        }
    }


    m_bodyManager->getBodyEdtor()->autoArrange();
    reset();
    return true;
}

bool AgentManager::saveConfig()
{
    m_brainEditor->updateItemLocations();
    m_bodyManager->getBodyEdtor()->updateItemLocations();
    QFile file(m_fileName);
    if(!file.open(QIODevice::WriteOnly) ) {
        return false;
    }
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("Agentconfig");
    stream.writeStartElement("Agent");
    stream.writeAttribute("name", m_name);
//    stream.writeAttribute("editorx", QString::number(m_editX));
//    stream.writeAttribute("editory", QString::number(m_editY));
    stream.writeStartElement("Body");
    foreach(quint32 id, m_bodyManager->getTraversedSegmentIds()) {
        Segment node=m_bodyManager->getSegment(id);
        //if(node) {
             stream.writeStartElement("Segment");
             stream.writeAttribute("id", QString::number(node.getId()));
             stream.writeAttribute("parent", QString::number(node.getParentId()));
             stream.writeAttribute("name", node.objectName());
             stream.writeAttribute("editorx",QString::number(m_editorSegmentNodeLocations.value(node.getId()).x()));
             stream.writeAttribute("editory",QString::number(m_editorSegmentNodeLocations.value(node.getId()).y()));
             stream.writeStartElement("Color");
             stream.writeAttribute("value", QString::number(node.getColor(),'f'));
             if(node.getColorInherited()) {
                 stream.writeAttribute("inherited", "true");
             } else {
                 stream.writeAttribute("inherited", "false");
             }
             stream.writeEndElement(); //Color

             stream.writeStartElement(BrainiacGlobals::XmlRestTranslationTag);
             stream.writeAttribute("x",  QString::number(node.getRestTranslation().x(),'f'));
             stream.writeAttribute("y",  QString::number(node.getRestTranslation().y(),'f'));
             stream.writeAttribute("z",  QString::number(node.getRestTranslation().z(),'f'));
             stream.writeEndElement(); //RestTranslation
             stream.writeStartElement(BrainiacGlobals::XmlRestRotationTag);
             stream.writeAttribute("x",  QString::number(node.getRestRotation().x(),'f'));
             stream.writeAttribute("y",  QString::number(node.getRestRotation().y(),'f'));
             stream.writeAttribute("z",  QString::number(node.getRestRotation().z(),'f'));
             stream.writeEndElement(); //RestRotation

             stream.writeStartElement(BrainiacGlobals::XmlTranslationTag);
             stream.writeAttribute("x",  QString::number(node.getTranslation().x(),'f'));
             stream.writeAttribute("y",  QString::number(node.getTranslation().y(),'f'));
             stream.writeAttribute("z",  QString::number(node.getTranslation().z(),'f'));
             stream.writeEndElement(); //Translation
             stream.writeStartElement(BrainiacGlobals::XmlRotationTag);
             stream.writeAttribute("x",  QString::number(node.getRotation().x(),'f'));
             stream.writeAttribute("y",  QString::number(node.getRotation().y(),'f'));
             stream.writeAttribute("z",  QString::number(node.getRotation().z(),'f'));
             stream.writeEndElement(); //Rotation

             stream.writeStartElement(BrainiacGlobals::XmlSizeTag);
             stream.writeAttribute("x",  QString::number(node.getSize().x(),'f'));
             stream.writeAttribute("y",  QString::number(node.getSize().y(),'f'));
             stream.writeAttribute("z",  QString::number(node.getSize().z(),'f'));
             stream.writeEndElement(); // Size

             stream.writeStartElement(BrainiacGlobals::XmlRotTransOrderTag);
             QString rotTrans;
             foreach(BrainiacGlobals::RotTrans rt,node.getRotationTranslationOrder()) {
                 if(rt==BrainiacGlobals::RX) {
                     rotTrans.append("RX");
                 } else if(rt==BrainiacGlobals::RY) {
                     rotTrans.append("RY");
                 } else if(rt==BrainiacGlobals::RZ) {
                     rotTrans.append("RZ");
                 } else if(rt==BrainiacGlobals::TX) {
                     rotTrans.append("TX");
                 } else if(rt==BrainiacGlobals::TY) {
                     rotTrans.append("TY");
                 } else if(rt==BrainiacGlobals::TZ) {
                     rotTrans.append("TZ");
                 }
                 rotTrans.append(" ");
             }
             stream.writeAttribute("order",rotTrans);

             stream.writeEndElement(); // RotTransOrder

             if(node.getType()==BrainiacGlobals::SPHERESEGMENT) {
                 stream.writeStartElement(BrainiacGlobals::XmlSphereTag);
                 stream.writeEndElement();
             } else if (node.getType()==BrainiacGlobals::BOXSEGMENT) {
                 stream.writeStartElement(BrainiacGlobals::XmlBoxTag);
                 stream.writeEndElement();
             } else if (node.getType()==BrainiacGlobals::TUBESEGMENT) {
                 stream.writeStartElement(BrainiacGlobals::XmlTubeTag);
                 stream.writeEndElement();
             } else {
                 qCritical() << __PRETTY_FUNCTION__<< "Error while saving: Unknown node type " << node.getType() << node.getName();
                 return false;
             }

             stream.writeEndElement(); // Segment
        //}
    }

    stream.writeEndElement(); // Body


    // MotionTree;
    m_motionTreeManager->saveConfig(&stream);

    stream.writeStartElement("Animations");
    QHashIterator<QString, Animation *> i(m_animations) ;

    while(i.hasNext()) {
        i.next();
        Animation *anim=i.value();
        if(anim->fileName().length()>0) {
            stream.writeStartElement("Animation");
            //stream.writeAttribute(BrainiacGlobals::XmlNameAttrib,i.key());
            QString relFileName=m_scene->getRelativeFileDir(anim->fileName());
            stream.writeAttribute(BrainiacGlobals::XmlFileNameAttrib,relFileName);
            stream.writeEndElement(); // Animation
        }
    }
    stream.writeEndElement(); // Animations

    stream.writeStartElement("Brain");
    foreach(FuzzyBase *fuzz,m_masterAgent->getBrain()->getFuzzies()) {
        if(fuzz->getType()==FuzzyBase::AND) {
            stream.writeStartElement("And");
            FuzzyAnd *fAnd=(FuzzyAnd *)fuzz;
            if(fAnd->getMode()==FuzzyAnd::MIN) {
                stream.writeAttribute(BrainiacGlobals::XmlModeAttrib, BrainiacGlobals::FuzzAndModeMin);
            } else {
                stream.writeAttribute(BrainiacGlobals::XmlModeAttrib, BrainiacGlobals::FuzzAndModeProduct);
            }
        } else if(fuzz->getType()==FuzzyBase::OR) {
            stream.writeStartElement("Or");
            FuzzyOr *fOr=(FuzzyOr *)fuzz;
            if(fOr->getMode()==FuzzyOr::MAX) {
                stream.writeAttribute(BrainiacGlobals::XmlModeAttrib, BrainiacGlobals::FuzzOrModeMax);
            } else {
                stream.writeAttribute(BrainiacGlobals::XmlModeAttrib, BrainiacGlobals::FuzzOrModeSum);
            }
        } else if(fuzz->getType()==FuzzyBase::INPUT) {
            stream.writeStartElement("Input");
            Input *inp=(Input *)fuzz;
            stream.writeAttribute("channel", inp->getChannelName());
            stream.writeAttribute("min",  QString::number(inp->getMinValue(),'f'));
            stream.writeAttribute("max",  QString::number(inp->getMaxValue(),'f'));
        }  else if(fuzz->getType()==FuzzyBase::DEFUZZ) {
            stream.writeStartElement("Defuzz");
            FuzzyDefuzz *defuzz=(FuzzyDefuzz *)fuzz;
            stream.writeAttribute("defuzzvalue",  QString::number(defuzz->getDefuzzVal(),'f'));
            if(defuzz->isElse()) {
                stream.writeAttribute("iselse", "true");
            } else {
                stream.writeAttribute("iselse", "false");
            }
        } else if(fuzz->getType()==FuzzyBase::OUTPUT) {
            stream.writeStartElement("Output");
            Output *out=(Output *)fuzz;
            stream.writeAttribute("channel", out->getChannelName());
            stream.writeAttribute("min",  QString::number(out->getMinValue(),'f'));
            stream.writeAttribute("max",  QString::number(out->getMaxValue(),'f'));
        } else if(fuzz->getType()==FuzzyBase::NOISE) {
            stream.writeStartElement("Noise");
            Noise *noise=(Noise *)fuzz;
            stream.writeAttribute("rate",  QString::number(noise->getRate(),'f'));
        } else if(fuzz->getType()==FuzzyBase::FUZZ) {
            stream.writeStartElement("Fuzz");
            FuzzyFuzz *fuzzy=(FuzzyFuzz *)fuzz;
            stream.writeAttribute("p1",  QString::number(fuzzy->getP1(),'f'));
            stream.writeAttribute("p2",  QString::number(fuzzy->getP2(),'f'));
            stream.writeAttribute("p3",  QString::number(fuzzy->getP3(),'f'));
            stream.writeAttribute("p4",  QString::number(fuzzy->getP4(),'f'));
            switch(fuzzy->getMode()) {
                case FuzzyFuzz::TRAPEZOID:
                    stream.writeAttribute(BrainiacGlobals::XmlModeAttrib, BrainiacGlobals::FuzzFuzzModeTrapezoid);
                    break;
                case FuzzyFuzz::TRIANGLE:
                    stream.writeAttribute(BrainiacGlobals::XmlModeAttrib, "triangle");
                    break;
                case FuzzyFuzz::ACTIVATE:
                    stream.writeAttribute(BrainiacGlobals::XmlModeAttrib, "activate");
                    break;
                case FuzzyFuzz::DEACTIVATE:
                    stream.writeAttribute(BrainiacGlobals::XmlModeAttrib, "deactivate");
                    break;
                case FuzzyFuzz::DIRAC:
                    stream.writeAttribute(BrainiacGlobals::XmlModeAttrib, "dirac");
                    break;
                default:
                    qDebug() << __PRETTY_FUNCTION__ <<" unknown Mode";
            }
            switch(fuzzy->getInterpolationMode()) {
                case FuzzyFuzz::LINEAR:
                    stream.writeAttribute("interpolation", "linear");
                    break;
                case FuzzyFuzz::SINE:
                    stream.writeAttribute("interpolation", "sine");
                    break;
                case FuzzyFuzz::QUAD:
                    stream.writeAttribute("interpolation", "quad");
                    break;
                default:
                    qDebug() << __PRETTY_FUNCTION__ <<" unknown Mode";
            }
        } else if(fuzz->getType()==FuzzyBase::TIMER) {
            stream.writeStartElement("Timer");
            Timer *timer=(Timer *)fuzz;
            stream.writeAttribute("rate",  QString::number(timer->getRate(),'f'));
            switch(timer->getMode()) {
            case Timer::IFSTOPPED:
                stream.writeAttribute(BrainiacGlobals::XmlModeAttrib,  BrainiacGlobals::FuzzTimerModeIfStopped);
                break;
            case Timer::ALWAYS:
                stream.writeAttribute(BrainiacGlobals::XmlModeAttrib,  "always");
                break;
            default:
                qDebug() << __PRETTY_FUNCTION__ <<" unknown Mode";
            }
        } else {
            stream.writeStartElement("Dummy");
            qDebug() << __PRETTY_FUNCTION__ << "Could not save fuzz with type" << fuzz->getType();
        }
        stream.writeAttribute("id",QString::number(fuzz->getId()));
        stream.writeAttribute("name",fuzz->getName());
        stream.writeAttribute("editorx",QString::number(m_editorFuzzyLocations.value(fuzz->getId()).x()));
        stream.writeAttribute("editory",QString::number(m_editorFuzzyLocations.value(fuzz->getId()).y()));
        stream.writeEndElement(); //FuzzyFuzz
    }
    foreach(FuzzyBase *fuzz,m_masterAgent->getBrain()->getFuzzies()) {
        foreach(FuzzyBase *childFuzz,fuzz->getChildren()) {
            stream.writeStartElement("Connector");
            stream.writeAttribute("parent",QString::number(fuzz->getId()));
            stream.writeAttribute("child",QString::number(childFuzz->getId()));
            if(childFuzz->isConnectionInverted(fuzz->getId())) {
                stream.writeAttribute("inverted", "1");
            } else {
                stream.writeAttribute("inverted", "0");
            }
            stream.writeEndElement(); //Connector
        }
    }
    stream.writeEndElement(); // Brain
    stream.writeEndElement(); // Agent
    stream.writeEndElement(); // Agentconfig

    stream.writeEndDocument();
    return true;
}

void AgentManager::reset()
{
    foreach(Agent *agent, m_agents) {
        agent->reset();
    }
}

void AgentManager::setEditorTranslation(qint32 x, qint32 y)
{
    m_editX=x;
    m_editY=y;
}

void AgentManager::setFuzzyEditorTranslation(quint32 id, qint32 x, qint32 y)
{
    QPoint point;//=m_editorFuzzyLocations.value(id);
    point.setX(x);
    point.setY(y);
    m_editorFuzzyLocations.insert(id,point);
}


void AgentManager::setBodyEditorTranslation(quint32 id, qint32 x, qint32 y)
{
    QPoint point;//=m_editorFuzzyLocations.value(id);
    point.setX(x);
    point.setY(y);
    m_editorSegmentNodeLocations.insert(id,point);
}

void AgentManager::setSegmentDimensions(quint32 id, qreal x, qreal y, qreal z)
{
    m_bodyManager->setSegmentSize(id,x,y,z);
}

void AgentManager::setSegmentRotationTranslationOrder(quint32 id, QList<BrainiacGlobals::RotTrans> list)
{
    if(!list.contains(BrainiacGlobals::RX))
        list.append(BrainiacGlobals::RX);
    if(!list.contains(BrainiacGlobals::RY))
        list.append(BrainiacGlobals::RY);
    if(!list.contains(BrainiacGlobals::RZ))
        list.append(BrainiacGlobals::RZ);

    if(!list.contains(BrainiacGlobals::TX))
        list.append(BrainiacGlobals::TX);
    if(!list.contains(BrainiacGlobals::TY))
        list.append(BrainiacGlobals::TY);
    if(!list.contains(BrainiacGlobals::TZ))
        list.append(BrainiacGlobals::TZ);

    m_bodyManager->setSegmentRotationTranslationOrder(id,list);
}

void AgentManager::setSegmentRestRotation(quint32 id, qreal x, qreal y, qreal z)
{
    m_bodyManager->setSegmentRestRotation(id,x,y,z);

    // the channels have to be reset
    this->reset();
}

void AgentManager::setSegmentRestTranslation(quint32 id, qreal x, qreal y, qreal z)
{
    //m_masterAgent->getBody()->getSkeletonNodeById(id)->setRestTranslation(QVector3D(x,y,z));
//    foreach(Agent *agent, m_agents) {
//        agent->getBody()->getSkeletonNodeById(id)->setRestTranslation(QVector3D(x,y,z));
//    }
//        qDebug()<< __PRETTY_FUNCTION__;
    m_bodyManager->setSegmentRestTranslation(id,x,y,z);

    // the channels have to be reset
    this->reset();
}

void AgentManager::setSegmentRotation(quint32 id, qreal x, qreal y, qreal z)
{
    //m_masterAgent->getBody()->getSkeletonNodeById(id)->setRotation(QVector3D(x,y,z));
//    foreach(Agent *agent, m_agents) {
//        //agent->getBody()->getSkeletonNodeById(id)->setRotation(QVector3D(x,y,z));
//    }
    m_bodyManager->setSegmentRotation(id,x,y,z);
}

void AgentManager::setSegmentTranslation(quint32 id, qreal x, qreal y, qreal z)
{
    //m_masterAgent->getBody()->getSkeletonNodeById(id)->setTranslation(QVector3D(x,y,z));
//    foreach(Agent *agent, m_agents) {
//        agent->getBody()->getSkeletonNodeById(id)->setTranslation(QVector3D(x,y,z));
//    }
    m_bodyManager->setSegmentTranslation(id,x,y,z);
}

void AgentManager::setFuzzyAndIsSoundRule(quint32 id, bool isSoundRule)
{
    if(m_masterAgent->getBrain()->getFuzzy(id)->getType()==FuzzyBase::AND) {
//        FuzzyAnd *fuzzy=(FuzzyAnd *)m_masterAgent->getBrain()->getFuzzy(id);
//        fuzzy->setIsSoundRule(isSoundRule);
        foreach(Agent *agent, m_agents) {
            FuzzyAnd *agentFuzz=(FuzzyAnd *) agent->getBrain()->getFuzzy(id);
            Q_ASSERT(agentFuzz->getType()==FuzzyBase::AND);
            agentFuzz->setIsSoundRule(isSoundRule);
        }
    }
}

void AgentManager::setFuzzyFuzzMode(quint32 id, FuzzyFuzz::Mode mode)
{
    if(m_masterAgent->getBrain()->getFuzzy(id)->getType()==FuzzyBase::FUZZ) {
//        FuzzyFuzz *fuzzy=(FuzzyFuzz *)m_masterAgent->getBrain()->getFuzzy(id);
//        fuzzy->setMode(mode);
        foreach(Agent *agent, m_agents) {
            FuzzyFuzz *agentFuzz=(FuzzyFuzz *) agent->getBrain()->getFuzzy(id);
            Q_ASSERT(agentFuzz->getType()==FuzzyBase::FUZZ);
            agentFuzz->setMode(mode);
        }
    }
}

void AgentManager::setFuzzyFuzzMembershipPoints(quint32 id, qreal p1, qreal p2, qreal p3, qreal p4)
{
    if(m_masterAgent->getBrain()->getFuzzy(id)->getType()==FuzzyBase::FUZZ) {
//        FuzzyFuzz *fuzzy=(FuzzyFuzz *)m_masterAgent->getBrain()->getFuzzy(id);
//        fuzzy->setP1(p1);
//        fuzzy->setP2(p2);
//        fuzzy->setP3(p3);
//        fuzzy->setP4(p4);
        foreach(Agent *agent, m_agents) {
            FuzzyFuzz *agentFuzz=(FuzzyFuzz *) agent->getBrain()->getFuzzy(id);
            Q_ASSERT(agentFuzz->getType()==FuzzyBase::FUZZ);
            agentFuzz->setP1(p1);
            agentFuzz->setP2(p2);
            agentFuzz->setP3(p3);
            agentFuzz->setP4(p4);
        }
    }
}

void AgentManager::setFuzzyFuzzInterpolationMode(quint32 id, FuzzyFuzz::InterpolationMode mode)
{
    if(m_masterAgent->getBrain()->getFuzzy(id)->getType()==FuzzyBase::FUZZ) {
//        FuzzyFuzz *fuzzy=(FuzzyFuzz *)m_masterAgent->getBrain()->getFuzzy(id);
//        fuzzy->setInterpolationMode(mode);
        foreach(Agent *agent, m_agents) {
            FuzzyFuzz *agentFuzz=(FuzzyFuzz *) agent->getBrain()->getFuzzy(id);
            Q_ASSERT(agentFuzz->getType()==FuzzyBase::FUZZ);
            agentFuzz->setInterpolationMode(mode);
        }
    }
}

void AgentManager::setFuzzyChannelName(quint32 id, QString name)
{
    FuzzyBase *fuzz=m_masterAgent->getBrain()->getFuzzy(id);

    switch(fuzz->getType()) {
//    Input *inp;
//    Output *out;

    case(FuzzyBase::INPUT):
//        inp=(Input *) fuzz;
//        inp->setChannelName(name);

        foreach(Agent *agent, m_agents) {
            Input *agentInput=(Input *) agent->getBrain()->getFuzzy(id);
            Q_ASSERT(agentInput->getType()==FuzzyBase::INPUT);
            agentInput->setChannelName(name);
        }
        break;
    case(FuzzyBase::OUTPUT):
//        out=(Output *)fuzz;
//        out->setChannelName(name);
        foreach(Agent *agent, m_agents) {
            Output *agentOut=(Output *) agent->getBrain()->getFuzzy(id);
            Q_ASSERT(agentOut->getType()==FuzzyBase::OUTPUT);
            agentOut->setChannelName(name);
        }
        break;
    default:
        qDebug()  << __PRETTY_FUNCTION__ << "Fuzz with id" << id << "is neither input nor output!";
    }
}

void AgentManager::setFuzzyMinMax(quint32 id, qreal min, qreal max)
{
//    m_masterAgent->getBrain()->getFuzzy(id)->setMin(min);
//    m_masterAgent->getBrain()->getFuzzy(id)->setMax(max);
    foreach(Agent *agent, m_agents) {
        agent->getBrain()->getFuzzy(id)->setMin(min);
        agent->getBrain()->getFuzzy(id)->setMax(max);
    }
}

void AgentManager::setFuzzyName(quint32 id, QString name)
{
//    m_masterAgent->getBrain()->getFuzzy(id)->setName(name);
    foreach(Agent *agent, m_agents) {
        agent->getBrain()->getFuzzy(id)->setName(name);
    }
}

void AgentManager::setFuzzyResult(quint32 id, qreal result)
{
//    m_masterAgent->getBrain()->getFuzzy(id)->setResult(result);
    foreach(Agent *agent, m_agents) {
        agent->getBrain()->getFuzzy(id)->setResult(result);
    }
}

// Defuzz Stuff
void AgentManager::setDefuzzValue(quint32 id, qreal value)
{
    if(m_masterAgent->getBrain()->getFuzzy(id)->getType()==FuzzyBase::DEFUZZ) {
//        FuzzyDefuzz *masterDefuzz=(FuzzyDefuzz *)m_masterAgent->getBrain()->getFuzzy(id);
//        masterDefuzz->setDefuzzValAbs(value);
        foreach(Agent *agent, m_agents) {
            FuzzyDefuzz *agentDefuzz=(FuzzyDefuzz *)agent->getBrain()->getFuzzy(id);
            Q_ASSERT(agentDefuzz->getType()==FuzzyBase::DEFUZZ);
            agentDefuzz->setDefuzzValAbs(value);
        }
    }
}

void AgentManager::setDefuzzIsElse(quint32 id, bool isElse)
{
    if(m_masterAgent->getBrain()->getFuzzy(id)->getType()==FuzzyBase::DEFUZZ) {
//        FuzzyDefuzz *masterDefuzz=(FuzzyDefuzz *)m_masterAgent->getBrain()->getFuzzy(id);
//        masterDefuzz->setElse(isElse);
        foreach(Agent *agent, m_agents) {
            FuzzyDefuzz *agentDefuzz=(FuzzyDefuzz *)agent->getBrain()->getFuzzy(id);
            Q_ASSERT(agentDefuzz->getType()==FuzzyBase::DEFUZZ);
            agentDefuzz->setElse(isElse);
        }
    }
}

// Output Stuff
void AgentManager::setOutputDefuzzMode(quint32 id, Output::DefuzzMode mode)
{
    if(m_masterAgent->getBrain()->getFuzzy(id)->getType()==FuzzyBase::OUTPUT) {
//        Output *masterOut=(Output *)m_masterAgent->getBrain()->getFuzzy(id);
//        masterOut->setDefuzzMode(mode);
        foreach(Agent *agent, m_agents) {
            Output *agentOut=(Output *) agent->getBrain()->getFuzzy(id);
            Q_ASSERT(agentOut->getType()==FuzzyBase::OUTPUT);
            agentOut->setDefuzzMode(mode);
        }
    }

}

// Noise Stuff
void AgentManager::setNoiseRate(quint32 id, qreal rate)
{
    if(m_masterAgent->getBrain()->getFuzzy(id)->getType()==FuzzyBase::NOISE) {
//        Noise *masterNoise=(Noise *)m_masterAgent->getBrain()->getFuzzy(id);
//        masterNoise->setRate(rate);
        foreach(Agent *agent, m_agents) {
            Noise *agentNoise=(Noise *) agent->getBrain()->getFuzzy(id);
            Q_ASSERT(agentNoise->getType()==FuzzyBase::NOISE);
            agentNoise->setRate(rate);
        }
    }
}


void AgentManager::updateSoundConfigs()
{
    //qDebug() << __PRETTY_FUNCTION__;
    bool currentTest;
    foreach(FuzzyBase *fuzz, m_masterAgent->getBrain()->getFuzzies()) {
        currentTest=true;
        if(fuzz->getType()==FuzzyBase::AND || fuzz->getType()==FuzzyBase::OR) {
            if(!fuzz->hasParents()) {
                currentTest=false;
            } else {
                foreach(FuzzyBase *parentOfAndOr,fuzz->getParents()) {

                    if(parentOfAndOr->getType()==FuzzyBase::FUZZ && currentTest) {
                        if(!parentOfAndOr->hasParents()) {
                            currentTest=false;
                        } else {

                            foreach(FuzzyBase *parentOfFuzz, parentOfAndOr->getParents()) {

                                if(parentOfFuzz->getType()==FuzzyFuzz::INPUT && !parentOfFuzz->hasParents()  && currentTest) {
                                    //qDebug() << "Found Sound config!" << parentOfFuzz->getName() << parentOfAndOr->getName();
                                } else {
                                    currentTest=false;
                                }

                            }
                        }
                    } else {
                        currentTest=false;
                    }

                }
            }



            if(fuzz->getType()==FuzzyBase::AND) {
                this->setFuzzyAndIsSoundRule(fuzz->getId(),currentTest);
                //qDebug() << __PRETTY_FUNCTION__ << "set" << currentTest;
            } else {  //!<  @todo SoundConfig
               // qDebug() << __PRETTY_FUNCTION__ << "Todo: OR Rule";
            }

            //currentTest=false;

        } else {
            continue;
        }
    }
}
AgentManager::~AgentManager() {
    delete(m_bodyManager);
    foreach(Agent *agent,m_agents) {
        agent->deleteLater();
    }
    m_brainEditor->deleteLater();
    m_motionTreeManager->deleteLater();
}
