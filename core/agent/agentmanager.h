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


#ifndef AGENTMANAGER_H
#define AGENTMANAGER_H

#include <QtCore>
#include "core/agent/brain/output.h"
#include "core/idgenerator.h"
#include "core/agent/brain/fuzzyfuzz.h"

class Agent;
class Group;
class QXmlStreamReader;
class Scene;
class Animation;
class BodyManager;

class AgentManager
{
public:
    AgentManager(Group *group);

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
                    @warning It is up to the user to manage and destroy this agent instance

            \param  id the id of the new agent
            \return pointer to new agent instance
    **/
    Agent* cloneAgent(quint32 id);

    /**
     * @brief creates a new managed instance of the agent
     *
     *      A new instance of an agent is created and is under control of this AgentManager.
     *      The pointer of the newly created Agent is returned
     * @fn createNewAgentInstance
     * @param id the id of the new agent
     * @return Agent pointer to the new agent instance
     */
    Agent* createNewAgentInstance(quint32 id);
    void deleteConnector(quint32 childId, quint32 parentId);

    /**
     * @brief returns a list of all agents managed by this AgentManager
     *
     * This list includes also the special purpose agents like master agent and
     * body agent
     * @fn getAllManagedAgents
     * @return QList<Agents *>
     */
    QList<Agent *> getAllManagedAgents() {return m_agents;}

    /** \brief deletes a fuzz from all agent´s brains belonging to this group

                    all connecttions from/to this fuzz´ brain are also deleted

            \param  fuzzId the id of the fuzz to be deleted
    **/
    void deleteFuzz(quint32 fuzzId);

    QHash<quint32,Animation *>* getAnimations() {return &m_animations;}
    quint32 getId() { return m_id; }
    qint32 getEditorTranslationX() { return m_editX; }
    qint32 getEditorTranslationY() { return m_editY; }
    QHash<quint32, QPoint> getEditorSegmentNodeLocations();
    QHash<quint32, QPoint> getEditorFuzzyLocations();
    Group* getGroup() { return m_group; }

    /**
     * @brief returns a pointer to the master gent
     *
     * The master agent is a special purpose agent. All new instances are created from this agent
     *
     * @fn getMasterAgent
     * @return Agent
     */
    Agent* getMasterAgent() {return m_masterAgent; }

    /**
     * @brief returns a pointer to the body agent
     *
     * The body agent is a special purpose agent. It is used by the BodyEditor.
     *
     * @sa getMasterAgent()
     * @fn getBodyAgent
     * @return Agent
     */
    Agent* getBodyAgent() {return m_spBodyAgent;}

    /**
     * @brief returns a pointer to the actions agent
     *
     * The action agent is a special purpose agent. It is used by the ActionEditor.
     *
     * @sa getMasterAgent()
     * @fn getActionAgent
     * @return Agent
     */
    Agent* getActionAgent() {return m_spActionAgent;}

    QString & getName() {return m_name;}
    QString getName() const { return m_name;}

    BodyManager *getBodyManager() { return m_bodyManager; }

    /**
     * @brief returns a modifiable reference to the segment id generator
     *
     * @sa BodyManager
     * @fn getSegmentIdGenerator
     * @return IdGenerator
     */
    IdGenerator& getSegmentIdGenerator() { return m_segmentIdGenerator; }
    /**
     * @brief check, if a filame is set
     *
     * @return bool true, if filename is set
     */
    bool isFileNameSet() const { return m_fileName.size()>0; }
    bool loadConfig();

    /**
     * @brief loads an animation
     *
     * @fn loadAnimation
     * @param filename the animation file to load
     * @return bool
     */
    bool loadAnimation(const QString &filename);
    /**
     * @brief loads a skeleton file

     * Loads a skeleton file. Currently only bvh file format is supported
     * @todo check and delete if a previous skeleton exists
     * @param filename the filename of the skeleton file
     * @return bool
     */
    bool loadSkeleton(const QString &filename);
    bool saveConfig();
    void setEditorTranslation(qint32 x, qint32 y);
    void setFuzzyEditorTranslation(quint32 id, qint32 x, qint32 y);
    void setBodyEditorTranslation(quint32 id, qint32 x, qint32 y);

    /**
     * @brief sets the dimensions of a primitive (except for a sphere)
     *
     * for a sphere call setSphereRadius instead
     * @fn setSegmentDimensions
     * @param id
     * @param x
     * @param y
     * @param z
     */
    void setSegmentDimensions(quint32 id, qreal x, qreal y, qreal z);

    /**
     * @brief sets the rest rotation of segment with given id

     *
     * @fn setSegmentRestRotation
     * @param id
     * @param x
     * @param y
     * @param z
     */
    void setSegmentRestRotation(quint32 id, qreal x, qreal y, qreal z);

    /**
     * @brief sets the order for rotations and translations
     *
     * if the array does not contain all 3 rotations or 3 translations, the missing transforms are added to the end
     * @fn setSegmentRotationTranslationOrder
     * @param id the id of the segment
     * @param list the list containing the transformation order
     */
    void setSegmentRotationTranslationOrder(quint32 id, QList <BrainiacGlobals::RotTrans> list );

    /**
     * @brief sets the rest translation of segment with given id

     *
     * @fn setSegmentRestTranslation
     * @param id
     * @param x
     * @param y
     * @param z
     */
    void setSegmentRestTranslation(quint32 id, qreal x, qreal y, qreal z);

    /**
     * @brief sets the rotation of segment with given id

     *
     * @fn setSegmentRotation
     * @param id
     * @param x
     * @param y
     * @param z
     */
    void setSegmentRotation(quint32 id, qreal x, qreal y, qreal z);

    /**
     * @brief sets the translation of segment with given id

     *
     * @fn setSegmentTranslation
     * @param id
     * @param x
     * @param y
     * @param z
     */
    void setSegmentTranslation(quint32 id, qreal x, qreal y, qreal z);

    /** \brief sets the sound rule state of the And fuzz of all agents
      @param id the and fuzz´ id
      @param isSoundRule true if this and fuzz shall be a sound rule fuzz
      @sa Defuzz
    **/
    void setFuzzyAndIsSoundRule(quint32 id, bool isSoundRule);

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

    /** \brief sets the fuzzification mode (membership) of given fuzz of all agents
      @param id the fuzz´ id
      @param mode the fuzzification mode
      @sa FuzzyFuzz::Mode
    **/
    void setFuzzyFuzzMode(quint32 id, FuzzyFuzz::Mode mode);

    /** \brief sets the interpolation mode  of given fuzz of all agents
      @param id the fuzz´ id
      @param mode the interpolation mode
      @sa FuzzyFuzz::InterpolationMode
    **/
    void setFuzzyFuzzInterpolationMode(quint32 id, FuzzyFuzz::InterpolationMode mode);

    /** \brief sets the membership points of given fuzz of all agents
      It is NOT checked, if point bounds are valid!
      @param id the fuzz´ id
      @param p1 first point of the membership curve
      @param p2 second point of the membership curve
      @param p3 third point of the membership curve
      @param p4 4th point of the membership curve
    **/
    void setFuzzyFuzzMembershipPoints(quint32 id, qreal p1, qreal p2, qreal p3, qreal p4);

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

    /** \brief sets the rate of given rate of all agents
                each agent belonging to this manager is updated including its master agent
                @param id the noise id
                @param rate the rate of this noise. A value of "0" returns a fixed pseudo random value
                @sa Noise
    **/
    void setNoiseRate(quint32 id, qreal rate);
    void setId(quint32 id) { m_id=id; }
    void setName( QString name ) { m_name=name; }
    void setFileName( QString fileName ) { m_fileName=fileName; }

    void updateSoundConfigs();
    ~AgentManager();
protected:
    Agent *m_masterAgent; /**< special purpose agent. The reference all other instances are created from */
    Agent *m_spBodyAgent; /**< special purpose agent. This agent will be used within the BodyEditor */
    Agent *m_spActionAgent; /**< special purpose agent. This agent will be used within the ActionEditor */
    BodyManager *m_bodyManager;
    QString m_name;
    QString m_fileName;
    quint32 m_id;
    quint32 m_editX, m_editY;
    Group *m_group;
    Scene *m_scene;
    QHash<quint32, QPoint> m_editorFuzzyLocations;
    QHash<quint32, QPoint> m_editorSegmentNodeLocations;
    IdGenerator m_brainIdGenerator;
    IdGenerator m_animationIdGenerator;
    IdGenerator m_segmentIdGenerator;
    QHash<quint32,Animation *> m_animations;
    QList<Agent *> m_agents; /**< contains all agents, including special purpose agents like @sa m_masterAgent */

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
    void addSegmentFromConfig( QXmlStreamReader *reader, quint32 id, QString name, quint32 parent, quint32 editorX, quint32 editorY );
    /**
     * @brief loads a bvh skeleton
     *
     * @param file the bvh file to load
     * @return bool true, if loading was successful
     * @todo color management, positioning of editoritems (optimized, no static offsets...), Site handling
     */
    bool loadSkeletonBVH(QFile &file);

    /**
     * @brief loads a bvh animation
     *
     * @fn loadAnmationBVH
     * @param file the bvh animation file to load
     * @return bool if loading was successful
     */
    bool loadAnmationBVH(QFile &file);
};

#endif // AGENTMANAGER_H
