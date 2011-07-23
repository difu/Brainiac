#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QVector3D>
#include "brain/fuzzyand.h" // needed for Mode
#include "brain/fuzzyor.h"
#include "brain/fuzzyfuzz.h"
#include "brain/timer.h"

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
    /** \brief adds an input channel

                    adds an input channel to the agent.
                    If channel already exists nothing is added
                    \return true, if adding was successful, false if not

    **/
    bool addInputChannel(Channel* channel, QString name);
    /** \brief adds an output channel

                    adds an output channel to the agent.
                    If channel already exists nothing is added
                    \param  channel pointer to output channel
                    @param name name of the channel
                    \return true, if adding was successful, false if not

    **/
    bool addOutputChannel(Channel* channel, QString name);
    void addConnection(quint32 child, quint32 parentId, bool inverted);
    // Fuzz Stuff
    void addAndFuzz(quint32 id, QString name, FuzzyAnd::Mode mode);
    void addDefuzz(quint32 id, QString name, qreal defuzzValue, bool isElse);
    void addFuzzFuzz(quint32 id, QString name, FuzzyFuzz::Mode mode, FuzzyFuzz::InterpolationMode intMode, qreal p1, qreal p2, qreal p3, qreal p4);
    void addInputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max);
    void addOrFuzz(quint32 id, QString name, FuzzyOr::Mode mode);
    void addOutputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max);
    void addNoiseFuzz(quint32 id, QString name, qreal rate);
    void addTimerFuzz(quint32 id, QString name, qreal rate, Timer::TimerMode mode);
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
    void advance();
    /** \brief commits all changes calculated by advance

                    all calculated channels etc are written down and are "baked", the new values becoming the actual values
                    @sa Agent::advance()

    **/
    void advanceCommit();

    /** \brief deletes channel

                    the channel is deleted both from inputs and outputs.
                    also its destructor is invoked
                    @param channel the channel to be deleted

    **/
    void deleteChannel(Channel* channel);
    /** \brief deletes a connection between two fuzzies
        @param childId the id of the child/the receiver
        @param parentId the id of the parent/the sender
    **/
    void deleteConnection(quint32 parentId, quint32 childId);

    /** \brief deletes a fuzz from this agent´s brain

                    all connecttions from/to this fuzz´ brain are also deleted

            \param  fuzzId the id of the fuzz to be deleted
    **/
    void deleteFuzz(quint32 fuzzId);

    /** \brief returns the body
    **/
    Body *getBody();
    /** \brief returns the brain
    **/
    Brain *getBrain();
    /** \brief returns the bodie´s color
            by default this value is inherited recursivly to all segments of the body
            @sa Segment::setColorInherited(bool inherited)
    **/
    Channel *getColor();
    /** \brief returns input channel
        @param name the name of the input channel
        @returns 0 if channel does not exist
    **/
    Channel *getInputChannel(QString name);
    /** \brief returns output channel
        @param name the name of the output channel
        @returns 0 if channel does not exist
    **/
    Channel *getOutputChannel(QString name);
    /** \brief returns the id of this agent
    **/
    quint32 getId();
    /** \brief returns the position of the agent in world space
    **/
    QVector3D *getPosition();
    /** \brief returns the rotation of the agent in world space
    **/
    QVector3D *getRotation();
    /** \brief true if sound emmisions should be rendered
    **/
    bool getRenderSoundEmission();
    /** \brief returns the scene
        @sa Scene
    **/
    Scene *getScene();
    /** \brief @returns true if channel exists
    **/
    bool inputChannelExists(QString name);
    /** \brief @returns true if channel exists
    **/
    bool outputChannelExists(QString name);
    /** \brief renders the agent in gl context
            calls the bodie´s render function
            it also draws sound emmissions
            @sa getRenderSoundEmission()
            @sa renderSoundEmission()
            @sa Body::renderGL()
    **/
    void renderGL();
    void renderSoundEmission(bool render);
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
    // Channels
    Channel *m_color; //!< color of agent (input and output)
    Channel *m_rx; //!< x rotation rate of agent (input and output)
    Channel *m_ry; //!< y rotation rate of agent (input and output)
    Channel *m_rz; //!< z rotation rate of agent (input and output)
    Channel *m_tx; //!< x translation rate of agent (input and output)
    Channel *m_ty; //!< y translation rate of agent (input and output)
    Channel *m_tz; //!< z translation rate of agent (input and output)
    Channel *m_oSoundF; //!< Emmited frequency of sound (output)
    Channel *m_oSoundA; //!< Emmited amplitude of sound (output)
    Channel *m_iSoundX; //!< polar coordinate about the Y axis of sound source (input)
    Channel *m_iSoundD; //!< distance of received sound;
    Channel *m_iSoundF; //!< frequency of received sound;
    Scene *m_scene; //!< the scene
    QVector3D m_position; //!< Agent position in world space
    QVector3D m_rotation; //!< Agent rotation in world space
    QVector3D m_newPosition; //!< new agent position in world space
    QVector3D m_newRotation; //!< new agent rotation in world space
    QVector3D m_restPosition; //!< Agent rest position in world space
    QVector3D m_restRotation; //!< Agent rest rotation in world space

    bool m_renderSoundEmission; //!< toogle render sound emissions

signals:

public slots:

};

#endif // AGENT_H
