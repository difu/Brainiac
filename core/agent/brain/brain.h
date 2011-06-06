#ifndef BRAIN_H
#define BRAIN_H

#include <QObject>

class Agent;
class FuzzyBase;
class Output;
class Input;


/** \brief  The agent´s brain

        Manages the agent´s brain

**/
class Brain : public QObject
{
    Q_OBJECT
public:
    explicit Brain(Agent *agent, Brain *brain);
    void addInputFuzz(Input *out);
    void addInputFuzz(quint32 id, QString name, QString channel);
    void addOutputFuzz(Output *out);
    void addOutputFuzz(quint32 id, QString name, QString channel);
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
