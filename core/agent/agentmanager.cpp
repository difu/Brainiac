#include "agentmanager.h"
#include "core/agent/agent.h"
#include "core/scene.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include "core/agent/body/sphere.h"
#include "core/agent/brain/brain.h"
#include "core/agent/brain/fuzzyand.h"
#include "core/agent/brain/fuzzyor.h"
#include "core/agent/brain/fuzzybase.h"
#include "core/agent/brain/input.h"
#include "core/agent/brain/output.h"
#include "core/agent/channel.h"
#include "core/group/group.h"
#include <QFile>
#include <QXmlStreamReader>
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
//    Input *in=(Input*)m_masterAgent->getBrain()->getFuzzy(id);
//    in->setMin(min);
//    in->setMax(max);
    foreach(Agent* agent,m_scene->getAgents()) {
        agent->addInputFuzz(id, name, channel, min, max);
//        Input *in=(Input*)agent->getBrain()->getFuzzy(id);
//        in->setMin(min);
//        in->setMax(max);
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

void AgentManager::addConnector(quint32 childId, quint32 parentId, bool inverted)
{
    m_masterAgent->addConnection(childId, parentId, inverted);
    foreach(Agent* agent,m_scene->getAgents()) {
        agent->addConnection(childId, parentId, inverted);
    }
}

/** \brief clones an agent

                this function clones an agent from this manager´s master agent

        \param  id the id of the new agent
        \return pointer to new agent instance
**/
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
                                    }else if(reader.name()=="Or") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addOrFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("mode").toString(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                                        reader.skipCurrentElement();
                                    }else if(reader.name()=="Noise") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        addNoiseFuzz(attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("rate").toString().toDouble(),attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
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
    return false;
}

void AgentManager::setEditorTranslation(qint32 x, qint32 y)
{
    m_editX=x;
    m_editY=y;
}

/** \brief sets result of a fuzz of all agents
            each agent belonging to this manager is updated including its master agent
**/
void AgentManager::setFuzzyResult(quint32 id, qreal result)
{
    m_masterAgent->getBrain()->getFuzzy(id)->setResult(result);
    foreach(Agent *agent, m_group->getAgents()) {
        agent->getBrain()->getFuzzy(id)->setResult(result);
    }
}
