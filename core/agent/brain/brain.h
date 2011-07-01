#ifndef BRAIN_H
#define BRAIN_H

#include <QObject>
#include "fuzzyand.h" // needed for Mode
#include "fuzzyor.h"
#include "fuzzyfuzz.h"

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
    void addAndFuzz(FuzzyAnd *andFuzz);
    void addAndFuzz(quint32 id, QString name, FuzzyAnd::Mode mode);
    void addDefuzz(FuzzyDefuzz *defuzz);
    void addDefuzz(quint32 id, QString name, qreal defuzzValue, bool isElse);
    void addFuzzFuzz(quint32 id, QString name, FuzzyFuzz::Mode mode, FuzzyFuzz::InterpolationMode intMode, qreal p1, qreal p2, qreal p3, qreal p4);
    void addFuzzFuzz(FuzzyFuzz *fuzzyFuzz);
    void addInputFuzz(Input *out);
    void addInputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max);
    void addOrFuzz(FuzzyOr *orFuzz);
    void addOrFuzz(quint32 id, QString name, FuzzyOr::Mode mode);
    void addOutputFuzz(Output *out);
    void addOutputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max);
    void addNoiseFuzz(Noise *noise);
    void addNoiseFuzz(quint32 id, QString name, qreal rate);
    void connectFuzzies(quint32 childId, quint32 parentId, bool inverted);
    Agent* getAgent();
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
