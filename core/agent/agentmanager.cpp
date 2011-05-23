#include "agentmanager.h"
#include "core/agent/agent.h"
#include "core/scene.h"
#include "core/agent/agent.h"
#include "core/agent/body/body.h"
#include "core/agent/body/sphere.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QVector3D>

AgentManager::AgentManager(Scene *scene)
{
    m_scene=scene;
    m_id=0;
}

void AgentManager::addSphereFromConfig(QXmlStreamReader *reader, quint32 id, QString name, quint32 parent)
{
    QVector3D *translation;
    QVector3D *rotation;
    qreal radius;

    while(reader->readNextStartElement()) {
        if(reader->name()=="Translation") {
            qDebug() << "Sphere Translation";
            QXmlStreamAttributes attribs = reader->attributes();
            translation= new QVector3D();
            translation->setX(attribs.value("x").toString().toDouble());
            translation->setY(attribs.value("y").toString().toDouble());
            translation->setZ(attribs.value("z").toString().toDouble());
            reader->skipCurrentElement();
        } else if(reader->name()=="Rotation") {
            qDebug() << "Sphere Rotation";
            QXmlStreamAttributes attribs = reader->attributes();
            rotation= new QVector3D();
            rotation->setX(attribs.value("x").toString().toDouble());
            rotation->setY(attribs.value("y").toString().toDouble());
            rotation->setZ(attribs.value("z").toString().toDouble());
            reader->skipCurrentElement();
        } else if(reader->name()=="Radius") {
            qDebug() << "Sphere Radius";
            QXmlStreamAttributes attribs = reader->attributes();
            radius=attribs.value("r").toString().toDouble();
            reader->skipCurrentElement();
        }
    }
    Segment *parentSeg=m_masterAgent->getBody()->getSegment(parent);
    if( !parentSeg )qDebug() << "Segment with id"<< id << "has no parent";
    Segment *seg=new Sphere(id, m_masterAgent->getBody(),name,rotation,translation,radius,parentSeg);
    m_masterAgent->getBody()->addSegment(seg);
    //reader->skipCurrentElement();

}

Agent* AgentManager::cloneAgent(quint32 id)
{
    Agent *agent=new Agent(m_scene,id);
    foreach(Segment *seg, m_masterAgent->getBody()->getSegments()) {
        if(seg->getType()==Segment::SPHERE) {
            Sphere *origSphere=(Sphere*)seg;
            QVector3D *rot=new QVector3D(origSphere->getRestRotation()->x(),origSphere->getRestRotation()->y(),origSphere->getRestRotation()->z());
            QVector3D *trans=new QVector3D(origSphere->getRestTranslation()->x(),origSphere->getRestTranslation()->y(),origSphere->getRestTranslation()->z());
            Sphere *newSphere=new Sphere(origSphere->getId(),agent->getBody(),origSphere->getName(),rot,trans,origSphere->getRestRadius());
            newSphere->setParent(origSphere->getParentId());
            agent->getBody()->addSegment(newSphere);
        }
    }
    return agent;
}

bool AgentManager::loadConfig()
{
    QFile file(m_fileName);
    if(file.open(QIODevice::ReadOnly)) {
        m_masterAgent=new Agent(m_scene,0); // Id 0 is ok, its just a master agent
        QXmlStreamReader reader;
        reader.setDevice(&file);
        while(reader.readNextStartElement()) {
            if(reader.name()=="Agentconfig") {
                while(reader.readNextStartElement()) {
                    if(reader.name()=="Agent") {
                        qDebug() << "Agemt Tag";
                        QXmlStreamAttributes attribs = reader.attributes();
                        setEditorTranslation(attribs.value("editorx").toString().toInt(),attribs.value("editory").toString().toInt());
                        setId(attribs.value("id").toString().toInt());
                        setName(attribs.value("name").toString());
                        while(reader.readNextStartElement()) {
                            if(reader.name()=="Body") {
                                qDebug() << "Body";
                                while(reader.readNextStartElement()) {
                                    qDebug()<< "Tag Name "<< reader.name();
                                    if(reader.name()=="Segment") {
                                        QXmlStreamAttributes attribs = reader.attributes();
                                        if( QString::compare( attribs.value("type").toString(),QString("sphere"),Qt::CaseInsensitive ) == 0 ) {
                                            qDebug() << "found SPhere" << attribs.value("id").toString();
                                            addSphereFromConfig(&reader,attribs.value("id").toString().toInt(),attribs.value("name").toString(),attribs.value("parent").toString().toInt());
                                            //reader.skipCurrentElement();
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
