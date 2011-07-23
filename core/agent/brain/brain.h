#ifndef BRAIN_H
#define BRAIN_H

#include <QObject>
#include "fuzzyand.h" // needed for Mode
#include "fuzzyor.h"
#include "fuzzyfuzz.h"
#include "timer.h"

class Agent;
class FuzzyBase;
class FuzzyDefuzz;
class Output;
class Input;
class Noise;


/** \brief  The agent´s brain

        Manages the agent´s brain

**/
class Brain : public QObject
{
    Q_OBJECT
public:
    explicit Brain(Agent *agent, Brain *brain);
    /** \brief adds an and fuzzy rule to this brain
                @param andFuzz the and rule to be added
    **/
    void addAndFuzz(FuzzyAnd *andFuzz);
    /** \brief adds an and fuzzy rule to this brain
    **/
    void addAndFuzz(quint32 id, QString name, FuzzyAnd::Mode mode);
    /** \brief adds a defuzz fuzzy rule to this brain
                @param defuzz the defuzz rule to be added
    **/
    void addDefuzz(FuzzyDefuzz *defuzz);
    /** \brief adds a defuzz rule to this brain
    **/
    void addDefuzz(quint32 id, QString name, qreal defuzzValue, bool isElse);
    void addFuzzFuzz(quint32 id, QString name, FuzzyFuzz::Mode mode, FuzzyFuzz::InterpolationMode intMode, qreal p1, qreal p2, qreal p3, qreal p4);
    void addFuzzFuzz(FuzzyFuzz *fuzzyFuzz);
    /** \brief adds an input to this brain
                @param input the input to be added
    **/
    void addInputFuzz(Input *out);
    /** \brief adds an input to this brain
                @param id the id of this fuzz node
                @param name the name of this fuzz node
                @param channel the name of the Channel this output is assigned to
                @param min the min value
                @param max the max value
    **/
    void addInputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max);
    /** \brief adds an or fuzzy rule to this brain
                @param orFuzz the or rule to be added
    **/
    void addOrFuzz(FuzzyOr *orFuzz);
    /** \brief adds an or fuzzy rule to this brain
                @param orFuzz the or rule to be added
    **/
    void addOrFuzz(quint32 id, QString name, FuzzyOr::Mode mode);
    /** \brief adds an output to this brain
    **/
    void addOutputFuzz(Output *out);
    /** \brief adds an output to this brain
    **/
    void addOutputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max);
    /** \brief adds a noise fuzz to this brain
                @param noise the noise to be added
    **/
    void addNoiseFuzz(Noise *noise);
    /** \brief adds a noise fuzz to this brain
                @param id the the id of this fuzz node
                @param name the name of this fuzz node
                @param the rate of this noise node
    **/
    void addNoiseFuzz(quint32 id, QString name, qreal rate);
    void addTimerFuzz(Timer *timer);
    void addTimerFuzz(quint32 id, QString name, qreal rate, Timer::TimerMode mode);

    /** \brief connect two fuzz nodes
                Connect a child to a parent. After the fuzzies are connected, a calculation of the child is issued to fetch the value of the parent(s)
                @param childId the fuzz to receive the output/result
                @param parentId the fuzz to send it´s result
                @param inverted if the parent´s result must be inverted
                @sa FuzzyBase
    **/
    void connectFuzzies(quint32 childId, quint32 parentId, bool inverted);

    /** \brief deletes a fuzz from this brain

                    all connecttions from/to this fuzz´ brain are also deleted

            \param  fuzzId the id of the fuzz to be deleted
    **/
    void deleteFuzz(quint32 fuzzId);

    /** \brief disconnect two fuzz nodes
                disconnects the fuzzies and removes their child/parent entries
                @param childId the fuzz to receive the output/result
                @param parentId the fuzz to send it´s result
                @sa FuzzyBase
    **/
    void disconnectFuzzies(quint32 childId, quint32 parentId);

    /** \brief returns the agent this brain belongs to

                    @returns the agent
    **/
    Agent* getAgent();
    /** \brief returns the pointer to the fuzzie´s is

                    @returns the pointer to the fuzz node

    **/
    FuzzyBase* getFuzzy(quint32 id);
    QList <FuzzyBase *> getFuzzies();
    ~Brain();

protected:
    Agent *m_agent; //!< This brain´s agent
    QList <FuzzyBase *> m_fuzzies;
signals:

public slots:

};

#endif // BRAIN_H
