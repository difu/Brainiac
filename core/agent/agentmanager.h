#ifndef AGENTMANAGER_H
#define AGENTMANAGER_H

#include <QtCore>
#include "core/agent/brain/output.h"
#include "core/idgenerator.h"

class Agent;
class Group;
class QXmlStreamReader;
class Scene;

class AgentManager
{
public:
    AgentManager(Scene *scene, Group *group);

    // Brain stuff
    /** \brief adds an And fuzz rule to all agent brains

                    creates an And fuzz node with name "And" and MIN mode

            \param  editorX x ordinate of label in editor
            \param  editorY y ordinate of label in editor
            \return id of the created rule
    **/
    quint32 addAndFuzz(quint32 editorX=0, quint32 editorY=0);
    quint32 addAndFuzz(QString name, QString mode, quint32 editorX, quint32 editorY);
    /** \brief adds a Fuzz fuzz rule to all agent brains

                    creates a fuzz fuzz node with name "Fuzz", "sine" interpolation and "trapezoid" mode.
                    The points of the trapezoid are at 0.3, 0.5, 0.7, 0.9

            \param  editorX x ordinate of label in editor
            \param  editorY y ordinate of label in editor
            \return id of the created rule
    **/
    quint32 addFuzzFuzz(quint32 editorX=0, quint32 editorY=0);
    quint32 addFuzzFuzz(QString name, QString mode, QString intMode, qreal p1, qreal p2, qreal p3, qreal p4, quint32 editorX, quint32 editorY);

    /** \brief adds a Defuzz fuzz rule to all agent brains

                    creates a defuzz fuzz node with name "defuzz" and defuzz weight 1.0 as a "no else" defuzz rule

            \param  editorX x ordinate of label in editor
            \param  editorY y ordinate of label in editor
            \return id of the created rule
    **/
    quint32 addDefuzz(quint32 editorX=0, quint32 editorY=0);
    quint32 addDefuzz(QString name, qreal defuzzValue, bool isElse, quint32 editorX, quint32 editorY);
    /** \brief adds an Or fuzz rule to all agent brains

                    creates an Or fuzz node with name "Or" and MAX mode

            \param  editorX x ordinate of label in editor
            \param  editorY y ordinate of label in editor
            \return id of the created rule
    **/
    quint32 addOrFuzz(quint32 editorX=0, quint32 editorY=0);
    quint32 addOrFuzz(QString name, QString mode, quint32 editorX, quint32 editorY);
    /** \brief adds an Output fuzz rule to all agent brains

                    creates an output fuzz node with name "Output", no channel assigned, and min and max are set to 0,1

            \param  editorX x ordinate of label in editor
            \param  editorY y ordinate of label in editor
            \return id of the created rule
    **/
    quint32 addOutputFuzz(quint32 editorX=0, quint32 editorY=0);
    quint32 addOutputFuzz(QString name, QString channel, qreal min, qreal max, quint32 editorX, quint32 editorY);
    /** \brief adds an Input fuzz rule to all agent brains

                    creates an input fuzz node with name "Input", no channel assigned, and min and max are set to 0,1

            \param  editorX x ordinate of label in editor
            \param  editorY y ordinate of label in editor
            \return id of the created rule
    **/
    quint32 addInputFuzz(quint32 editorX=0, quint32 editorY=0);
    quint32 addInputFuzz(QString name, QString channel, qreal min, qreal max, quint32 editorX, quint32 editorY);
    /** \brief adds a Noise fuzz rule to all agent brains

                    creates a noise fuzz node with name "Noise" and a rate of "1"

            \param  editorX x ordinate of label in editor
            \param  editorY y ordinate of label in editor
            \return id of the created rule
    **/
    quint32 addNoiseFuzz(quint32 editorX=0, quint32 editorY=0);
    quint32 addNoiseFuzz(QString name, qreal rate, quint32 editorX, quint32 editorY);
    /** \brief adds a Timer fuzz rule to all agent brains

                    creates a timer fuzz node with name "Timer", a rate of "1" and mode "IF STOPPED"

            \param  editorX x ordinate of label in editor
            \param  editorY y ordinate of label in editor
            \return id of the created rule
    **/
    quint32 addTimerFuzz(quint32 editorX=0, quint32 editorY=0);
    quint32 addTimerFuzz(QString name, qreal rate, QString mode, quint32 editorX, quint32 editorY);
    void addConnector(quint32 childId, quint32 parentId, bool inverted);

    /** \brief clones an agent

                    this function clones an agent from this manager´s master agent

            \param  id the id of the new agent
            \return pointer to new agent instance
    **/
    Agent *cloneAgent(quint32 id);
    quint32 getId() { return m_id; }
    qint32 getEditorTranslationX() { return m_editX; }
    qint32 getEditorTranslationY() { return m_editY; }
    QHash<quint32, QPoint> getEditorFuzzyLocations();
    Group* getGroup() { return m_group; }
    Agent* getMasterAgent() {return m_masterAgent; }
    QString & getName() {return m_name;}
    bool loadConfig();
    bool saveConfig();
    void setEditorTranslation(qint32 x, qint32 y);
    void setFuzzyEditorTranslation(quint32 id, qint32 x, qint32 y);

    /** \brief sets the is else state of the defuzz of all agents
      @param id the defuzz´ id
      @param isElse true if this defuzz shall be an else defuzz
      @sa Defuzz
    **/
    void setDefuzzIsElse(quint32 id, bool isElse);

    /** \brief sets the defuzzification value of given defuzz of all agents
      @param id the defuzz´ id
      @param value the defuzz value
      @sa Defuzz
    **/
    void setDefuzzValue(quint32 id, qreal value);

    /** \brief sets the channel name of an input/output fuzz of all agents
      @param id the fuzz´ id
      @param name the name of the channel to set
    **/
    void setFuzzyChannelName( quint32 id, QString name);

    /** \brief sets the min and the max values of a fuzz of all agents
      @param id the fuzz´ id
      @param min minimum value of the fuzz´ result
      @param max maximum value of the fuzz´ result
    **/
    void setFuzzyMinMax(quint32 id, qreal min, qreal max);

    /** \brief sets the name of a fuzz of all agents
                each agent belonging to this manager is updated including its master agent
    **/
    void setFuzzyName(quint32 id,QString name);

    /** \brief sets result of a fuzz of all agents
                each agent belonging to this manager is updated including its master agent
    **/
    void setFuzzyResult(quint32 id, qreal result);

    /** \brief sets the dufuzzification mode of given output of all agents
                each agent belonging to this manager is updated including its master agent
                @param id the fuzz id
                @param mode the defuzzification mode
                @sa Output
                @sa Defuzz
    **/
    void setOutputDefuzzMode(quint32 id, Output::DefuzzMode mode);
    void setId(quint32 id) { m_id=id; }
    void setName( QString name ) { m_name=name; }
    void setFileName( QString fileName ) { m_fileName=fileName; }

protected:
    Agent *m_masterAgent;
    QString m_name;
    QString m_fileName;
    quint32 m_id;
    quint32 m_editX, m_editY;
    Group *m_group;
    Scene *m_scene;
    QHash<quint32, QPoint> m_editorFuzzyLocations;
    IdGenerator m_brainIdGenerator;

    // Brain stuff
    void addAndFuzz(quint32 id, QString name, QString mode, quint32 editorX, quint32 editorY);
    void addFuzzFuzz(quint32 id, QString name, QString mode, QString intMode, qreal p1, qreal p2, qreal p3, qreal p4, quint32 editorX, quint32 editorY);
    void addDefuzz(quint32 id, QString name, qreal defuzzValue, bool isElse, quint32 editorX, quint32 editorY);
    void addOrFuzz(quint32 id, QString name, QString mode, quint32 editorX, quint32 editorY);
    void addOutputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max, quint32 editorX, quint32 editorY);
    void addInputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max, quint32 editorX, quint32 editorY);
    void addNoiseFuzz(quint32 id, QString name, qreal rate, quint32 editorX, quint32 editorY);
    void addTimerFuzz(quint32 id, QString name, qreal rate, QString mode, quint32 editorX, quint32 editorY);

    // Body stuff
    void addSphereFromConfig( QXmlStreamReader *reader, quint32 id, QString name, quint32 parent );

};

#endif // AGENTMANAGER_H
