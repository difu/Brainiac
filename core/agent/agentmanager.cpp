#include "agentmanager.h"
#include "core/agent/agent.h"
#include "core/scene.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include "core/agent/body/sphere.h"
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
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QVector3D>

AgentManager::AgentManager(Scene *scene, Group *group)
{
    m_scene=scene;
    m_group=group;
    m_id=0;
    m_masterAgent=new Agent(m_scene,0); // Id 0 is ok, its just a master agent
}

void AgentManager::addSphereFromConfig(QXmlStreamReader *reader, quint32 id, QString name, quint32 parent)
{
    QVector3D *translation;
    QVector3D *rotation;
    qreal radius;
    qreal color;
    bool colorInherited;

    while(reader->readNextStartElement()) {
        if(reader->name()=="Translation") {
            QXmlStreamAttributes attribs = reader->attributes();
            translation= new QVector3D();
            translation->setX(attribs.value("x").toString().toDouble());
            translation->setY(attribs.value("y").toString().toDouble());
            translation->setZ(attribs.value("z").toString().toDouble());
            reader->skipCurrentElement();
        } else if(reader->name()=="Rotation") {
            QXmlStreamAttributes attribs = reader->attributes();
            rotation= new QVector3D();
            rotation->setX(attribs.value("x").toString().toDouble());
            rotation->setY(attribs.value("y").toString().toDouble());
            rotation->setZ(attribs.value("z").toString().toDouble());
            reader->skipCurrentElement();
        } else if(reader->name()=="Radius") {
            QXmlStreamAttributes attribs = reader->attributes();
            radius=attribs.value("r").toString().toDouble();
            reader->skipCurrentElement();
        }  else if(reader->name()=="Color") {
            QXmlStreamAttributes attribs = reader->attributes();
            color=attribs.value("value").toString().toDouble();
            colorInherited=attribs.value("inherited").toString().compare("true",Qt::CaseInsensitive)==0;
            reader->skipCurrentElement();
        }
    }
    Segment *parentSeg=m_masterAgent->getBody()->getSegment(parent);

    Segment *seg=new Sphere(id, m_masterAgent->getBody(),name,rotation,translation,radius,0);
    seg->setColorInherited(colorInherited);
    seg->setRestColor(color);
    seg->getColor()->init(color);
    if( parentSeg ) {
        seg->setParentId(parentSeg->getId());
    }
    m_masterAgent->getBody()->addSegment(seg);
    //reader->skipCurrentElement();

}

void AgentManager::addAndFuzz(quint32 id, QString name, QString mode, quint32 editorX, quint32 editorY)
{
    FuzzyAnd::Mode andMode;
    if(QString::compare("min",mode,Qt::CaseInsensitive)==0) {
        andMode=FuzzyAnd::MIN;
    } else {
        andMode=FuzzyAnd::PRODUCT;
    }
    m_masterAgent->addAndFuzz(id,name,andMode);
    foreach(Agent* agent,m_scene->getAgents()) {
        agent->addAndFuzz(id,name,andMode);
    }
    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
}

void AgentManager::addFuzzFuzz(quint32 id, QString name, QString mode, QString intMode, qreal p1, qreal p2, qreal p3, qreal p4, quint32 editorX, quint32 editorY)
{
    FuzzyFuzz::Mode fuzzMode=FuzzyFuzz::TRAPEZOID;;
    FuzzyFuzz::InterpolationMode interMode=FuzzyFuzz::LINEAR;
    if(QString::compare("trapezoid",mode,Qt::CaseInsensitive )==0) {
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
    } else if(QString::compare("sine",intMode,Qt::CaseInsensitive )==0) {
        interMode=FuzzyFuzz::SINE;
    } else if(QString::compare("quad",intMode,Qt::CaseInsensitive )==0) {
        interMode=FuzzyFuzz::QUAD;
    }

    m_masterAgent->addFuzzFuzz(id,name,fuzzMode,interMode,p1,p2,p3,p4);
    foreach(Agent* agent,m_scene->getAgents()) {
        agent->addFuzzFuzz(id,name,fuzzMode,interMode,p1,p2,p3,p4);
    }
    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
}

void AgentManager::addDefuzz(quint32 id, QString name, qreal defuzzValue, bool isElse, quint32 editorX, quint32 editorY)
{
    m_masterAgent->addDefuzz(id,name,defuzzValue,isElse);
    foreach(Agent* agent,m_scene->getAgents()) {
        agent->addDefuzz(id,name,defuzzValue,isElse);
    }
    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
}

void AgentManager::addOrFuzz(quint32 id, QString name, QString mode, quint32 editorX, quint32 editorY)
{
    FuzzyOr::Mode orMode;
    if(QString::compare("max",mode,Qt::CaseInsensitive)==0) {
        orMode=FuzzyOr::MAX;
    } else {
        orMode=FuzzyOr::SUM;
    }
    m_masterAgent->addOrFuzz(id,name,orMode);
    foreach(Agent* agent,m_scene->getAgents()) {
        agent->addOrFuzz(id,name,orMode);
    }
    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
}

void AgentManager::addOutputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max, quint32 editorX, quint32 editorY)
{
    m_masterAgent->addOutputFuzz(id, name, channel, min, max);
    //Output *out=(Output*)m_masterAgent->getBrain()->getFuzzy(id);
    foreach(Agent* agent,m_scene->getAgents()) {
        agent->addOutputFuzz(id, name, channel, min, max);
        //Output *out=(Output*)agent->getBrain()->getFuzzy(id);
    }

    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
}

void AgentManager::addInputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max, quint32 editorX, quint32 editorY)
{
    m_masterAgent->addInputFuzz(id, name, channel, min, max);
    foreach(Agent* agent,m_scene->getAgents()) {
        agent->addInputFuzz(id, name, channel, min, max);
    }

    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
}

void AgentManager::addNoiseFuzz(quint32 id, QString name, qreal rate, quint32 editorX, quint32 editorY)
{
    m_masterAgent->addNoiseFuzz(id, name, rate);
    //Noise *noise=(Noise*) m_masterAgent->getBrain()->getFuzzy(id);
    foreach(Agent* agent,m_scene->getAgents()) {
        agent->addNoiseFuzz(id, name, rate);
    }

    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
}
// addTimerFuzz(quint32 id, QString name, qreal rate, QString mode, quint32 editorX, quint32 editorY);
void AgentManager::addTimerFuzz(quint32 id, QString name, qreal rate, QString mode, quint32 editorX, quint32 editorY)
{
    if(QString::compare("ifstopped",mode,Qt::CaseInsensitive)==0) {
        m_masterAgent->addTimerFuzz(id,name,rate,Timer::IFSTOPPED);
        foreach(Agent* agent,m_scene->getAgents()) {
            agent->addTimerFuzz(id,name,rate,Timer::IFSTOPPED);
        }
        qDebug() << "Timwer " << name;
    }
    m_editorFuzzyLocations.insert(id,QPoint(editorX,editorY));
}

void AgentManager::addConnector(quint32 childId, quint32 parentId, bool inverted)
{
    m_masterAgent->addConnection(childId, parentId, inverted);
    foreach(Agent* agent,m_scene->getAgents()) {
        agent->addConnection(childId, parentId, inverted);
    }
}

Agent* AgentManager::cloneAgent(quint32 id)
{
    Agent *agent=new Agent(m_masterAgent,id);
    return agent;
}

QHash<quint32, QPoint> AgentManager::getEditorFuzzyLocations()
{
    return m_editorFuzzyLocations;
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
                        setEditorTranslation(attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                        setId(attribs.value("id").toString().toInt());
                        setName(attribs.value("name").toString());
                        while(reader.readNextStartElement()) {
                            if(reader.name()=="Body") {
                                while(reader.readNextStartElement()) {
                                    if(reader.name()=="Segment") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        if( QString::compare( attribs.value("type").toString(),QString("sphere"),Qt::CaseInsensitive ) == 0 ) {
                                            addSphereFromConfig(&reader,attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("parent").toString().toInt());
                                            //reader.skipCurrentElement();
                                        }

                                    }else {
                                        reader.skipCurrentElement();
                                    }
                                }
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
                                        addAndFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("mode").toString(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Defuzz") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addDefuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("defuzzvalue").toString().toDouble(), QString::compare(attribs.value("iselse").toString(),QString("true"),Qt::CaseInsensitive )==0,attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Or") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addOrFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("mode").toString(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Noise") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addNoiseFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("rate").toString().toDouble(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Fuzz") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addFuzzFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("mode").toString(),attribs.value("interpolation").toString(),attribs.value("p1").toString().toDouble(),attribs.value("p2").toString().toDouble(),attribs.value("p3").toString().toDouble(),attribs.value("p4").toString().toDouble(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Timer") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addTimerFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("rate").toString().toDouble(),attribs.value("mode").toString(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Connector") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addConnector(attribs.value("child").toString().toUInt(),attribs.value("parent").toString().toUInt(),attribs.value("inverted").toString().toUInt()!=0);
                                        reader.skipCurrentElement();
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
            } else {
                reader.skipCurrentElement();
            }
        }
        return true;
    }
    return false;
}

bool AgentManager::saveConfig()
{
    QFile file(m_fileName);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("Agentconfig");
    stream.writeStartElement("Agent");
    stream.writeAttribute("name", m_name);
    stream.writeAttribute("editorx", QString::number(m_editX));
    stream.writeAttribute("editory", QString::number(m_editY));
    stream.writeStartElement("Body");
    foreach(Segment *seg,m_masterAgent->getBody()->getSegments()) {
        stream.writeStartElement("Segment");
        stream.writeAttribute("id", QString::number(seg->getId()));
        stream.writeAttribute("parent", QString::number(seg->getParentId()));
        stream.writeAttribute("name", seg->getName());
        if(seg->getType()==Segment::SPHERE) {
            Sphere *sphere=(Sphere *)seg;
            stream.writeAttribute("type", "sphere");
            stream.writeStartElement("Radius");
            stream.writeAttribute("r", QString::number(sphere->getRestRadius(),'f'));
            stream.writeEndElement(); // Radius
        }
        stream.writeStartElement("Color");
        stream.writeAttribute("value", QString::number(seg->getRestColor(),'f'));
        if(seg->getColor()->isInherited()) {
            stream.writeAttribute("inherited", "true");
        } else {
            stream.writeAttribute("inherited", "false");
        }
        stream.writeEndElement(); //Color
        stream.writeStartElement("Translation");
        stream.writeAttribute("x",  QString::number(seg->getRestTranslation()->x(),'f'));
        stream.writeAttribute("y",  QString::number(seg->getRestTranslation()->y(),'f'));
        stream.writeAttribute("z",  QString::number(seg->getRestTranslation()->z(),'f'));
        stream.writeEndElement(); //Translation
        stream.writeStartElement("Rotation");
        stream.writeAttribute("x",  QString::number(seg->getRestRotation()->x(),'f'));
        stream.writeAttribute("y",  QString::number(seg->getRestRotation()->y(),'f'));
        stream.writeAttribute("z",  QString::number(seg->getRestRotation()->z(),'f'));
        stream.writeEndElement(); //Rotation

        stream.writeEndElement(); //Segment
    }
    stream.writeEndElement(); // Body
    stream.writeStartElement("Brain");
    foreach(FuzzyBase *fuzz,m_masterAgent->getBrain()->getFuzzies()) {
        if(fuzz->getType()==FuzzyBase::AND) {
            stream.writeStartElement("And");
            FuzzyAnd *fAnd=(FuzzyAnd *)fuzz;
            if(fAnd->getMode()==FuzzyAnd::MIN) {
                stream.writeAttribute("mode", "min");
            } else {
                stream.writeAttribute("mode", "prod");
            }
        } else if(fuzz->getType()==FuzzyBase::OR) {
            stream.writeStartElement("Or");
            FuzzyOr *fOr=(FuzzyOr *)fuzz;
            if(fOr->getMode()==FuzzyOr::MAX) {
                stream.writeAttribute("mode", "max");
            } else {
                stream.writeAttribute("mode", "sum");
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
                    stream.writeAttribute("mode", "trapezoid");
                    break;
                case FuzzyFuzz::TRIANGLE:
                    stream.writeAttribute("mode", "triangle");
                    break;
                case FuzzyFuzz::ACTIVATE:
                    stream.writeAttribute("mode", "activate");
                    break;
                case FuzzyFuzz::DEACTIVATE:
                    stream.writeAttribute("mode", "deactivate");
                    break;
                case FuzzyFuzz::DIRAC:
                    stream.writeAttribute("mode", "dirac");
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
                stream.writeAttribute("mode",  "ifstopped");
                break;
            case Timer::ALWAYS:
                stream.writeAttribute("mode",  "always");
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
    return false;
}

void AgentManager::setEditorTranslation(qint32 x, qint32 y)
{
    m_editX=x;
    m_editY=y;
}

void AgentManager::setFuzzyEditorTranslation(quint32 id, qint32 x, qint32 y)
{
    QPoint point=m_editorFuzzyLocations.value(id);
    point.setX(x);
    point.setY(y);
    m_editorFuzzyLocations.insert(id,point);
}

void AgentManager::setFuzzyChannelName(quint32 id, QString name)
{
    FuzzyBase *fuzz=m_masterAgent->getBrain()->getFuzzy(id);
    switch(fuzz->getType()) {
    Input *inp;
    Output *out;
    case(FuzzyBase::INPUT):
        inp=(Input *) fuzz;
        inp->setChannelName(name);
        foreach(Agent *agent, m_group->getAgents()) {
            Input *agentInput=(Input *) agent->getBrain()->getFuzzy(id);
            Q_ASSERT(agentInput->getType()==FuzzyBase::INPUT);
            agentInput->setChannelName(name);
        }
        break;
    case(FuzzyBase::OUTPUT):
        out=(Output *)fuzz;
        out->setChannelName(name);
        foreach(Agent *agent, m_group->getAgents()) {
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
    m_masterAgent->getBrain()->getFuzzy(id)->setMin(min);
    m_masterAgent->getBrain()->getFuzzy(id)->setMax(max);
    foreach(Agent *agent, m_group->getAgents()) {
        agent->getBrain()->getFuzzy(id)->setMin(min);
        agent->getBrain()->getFuzzy(id)->setMax(max);
    }
}

void AgentManager::setFuzzyName(quint32 id, QString name)
{
    m_masterAgent->getBrain()->getFuzzy(id)->setName(name);
    foreach(Agent *agent, m_group->getAgents()) {
        agent->getBrain()->getFuzzy(id)->setName(name);
    }
}

void AgentManager::setFuzzyResult(quint32 id, qreal result)
{
    m_masterAgent->getBrain()->getFuzzy(id)->setResult(result);
    foreach(Agent *agent, m_group->getAgents()) {
        agent->getBrain()->getFuzzy(id)->setResult(result);
    }
}

// Defuzz Stuff
void AgentManager::setDefuzzValue(quint32 id, qreal value)
{
    if(m_masterAgent->getBrain()->getFuzzy(id)->getType()==FuzzyBase::DEFUZZ) {
        FuzzyDefuzz *masterDefuzz=(FuzzyDefuzz *)m_masterAgent->getBrain()->getFuzzy(id);
        masterDefuzz->setDefuzzValAbs(value);
        foreach(Agent *agent, m_group->getAgents()) {
            FuzzyDefuzz *agentDefuzz=(FuzzyDefuzz *)agent->getBrain()->getFuzzy(id);
            Q_ASSERT(agentDefuzz->getType()==FuzzyBase::DEFUZZ);
            agentDefuzz->setDefuzzValAbs(value);
        }
    }
}

void AgentManager::setDefuzzIsElse(quint32 id, bool isElse)
{
    if(m_masterAgent->getBrain()->getFuzzy(id)->getType()==FuzzyBase::DEFUZZ) {
        FuzzyDefuzz *masterDefuzz=(FuzzyDefuzz *)m_masterAgent->getBrain()->getFuzzy(id);
        masterDefuzz->setElse(isElse);
        foreach(Agent *agent, m_group->getAgents()) {
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
        Output *masterOut=(Output *)m_masterAgent->getBrain()->getFuzzy(id);
        masterOut->setDefuzzMode(mode);
        foreach(Agent *agent, m_group->getAgents()) {
            Output *agentOut=(Output *) agent->getBrain()->getFuzzy(id);
            Q_ASSERT(agentOut->getType()==FuzzyBase::OUTPUT);
            agentOut->setDefuzzMode(mode);
        }
    }

}
