#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QVector3D>

class Body;
class Brain;
class Channel;
class Scene;
class Segment;

/** \brief  Main agent class

        This class holds all properties of an agent.
        It holds the agent´s brain, actions and body.
        The class also makes the agent interact with other scene content
        like other agents, terrain, colors etc.
        @sa Body
        @sa Brain
        @sa Scene

**/
class Agent : public QObject
{
    Q_OBJECT
public:
    Agent(Scene *scene, quint32 id);
    Agent(Agent *agent, quint32 id=0);
    bool addInputChannel(Channel* channel, QString name);
    bool addOutputChannel(Channel* channel, QString name);
    void addOutputFuzz(quint32 id, QString name, QString channel);
    void advance();
    void advanceCommit();
    void deleteChannel(Channel* channel);
    Body *getBody();
    Brain *getBrain();
    Channel *getColor();
    Channel *getInputChannel(QString name);
    Channel *getOutputChannel(QString name);
    quint32 getId();
    QVector3D *getPosition();
    QVector3D *getRotation();
    Scene *getScene();
    bool inputChannelExists(QString name);
    bool outputChannelExists(QString name);
    void renderGL();
    void reset();
    void setRotation(qreal x, qreal y, qreal z);
    void setTranslation(qreal x, qreal y, qreal z);
    void setRestRotation(qreal x, qreal y, qreal z);
    void setRestTranslation(qreal x, qreal y, qreal z);
    virtual ~Agent();

protected:
    Body *m_body; //!< the agent´s body
    Brain *m_brain; //!< the agent´s brain
    quint32 m_id; //!< the agent´s id. Unique for its group
    QHash<QString, Channel *> m_inputs; //!< List of all input channels of this agent
    QHash<QString, Channel *> m_outputs; //!< List of all output channels of this agent
    void createChannels();
    Channel *m_color; //!< color of agent (input and output)
    Channel *m_rx; //!< x rotation rate of agent (input and output)
    Channel *m_ry; //!< y rotation rate of agent (input and output)
    Channel *m_rz; //!< z rotation rate of agent (input and output)
    Channel *m_tx; //!< x translation rate of agent (input and output)
    Channel *m_ty; //!< y translation rate of agent (input and output)
    Channel *m_tz; //!< z translation rate of agent (input and output)
    Scene *m_scene; //!< the scene
    QVector3D m_position; //!< Agent position in world space
    QVector3D m_rotation; //!< Agent rotation in world space
    QVector3D m_newPosition; //!< new agent position in world space
    QVector3D m_newRotation; //!< new agent rotation in world space
    QVector3D m_restPosition; //!< Agent rest position in world space
    QVector3D m_restRotation; //!< Agent rest rotation in world space

signals:

public slots:

};

#endif // AGENT_H
