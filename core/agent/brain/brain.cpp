#include "brain.h"
#include "core/agent/agent.h"
#include "core/agent/brain/output.h"

Brain::Brain(Agent *agent, Brain *brain) :
    QObject(),m_agent(agent)
{
    if(brain) {
        foreach(FuzzyBase *fuzz,brain->getFuzzies()) {
            if(fuzz->getType()==FuzzyBase::OUTPUT) {
                Output *origOut=(Output *)fuzz;
                addOutputFuzz(origOut->getId(),origOut->getName(),origOut->getChannelName());
                Output *out=(Output*)brain->getFuzzy(origOut->getId());
                out->setMin(origOut->getMinValue());
                out->setMax(origOut->getMaxValue());
            } else {
                qDebug() <<  __PRETTY_FUNCTION__ << "missing fuzz type" << fuzz->getId();
            }
        }
    }
}


/** \brief adds an output to this brain
            @param the output to be added
**/
void Brain::addOutputFuzz(Output *out)
{
    m_fuzzies.append(out);
}

/** \brief adds an output to this brain
**/
void Brain::addOutputFuzz(quint32 id, QString name, QString channel)
{
    Output *out=new Output(id, this, name, channel);
    addOutputFuzz(out);
}


/** \brief returns the agent this brain belongs to

                @returns the agent

**/
Agent* Brain::getAgent()
{
    return m_agent;
}

/** \brief get this fuzzie´s id

                @returns the fuzzie´s id

**/
FuzzyBase* Brain::getFuzzy(quint32 id)
{
    foreach(FuzzyBase *fuzzy,m_fuzzies) {
        if(fuzzy->getId()==id) {
            return fuzzy;
        }
    }
    return 0;
}
QList<FuzzyBase *> Brain::getFuzzies()
{
    return m_fuzzies;
}

Brain::~Brain()
{
    foreach(FuzzyBase *fuzz, m_fuzzies) {
        delete fuzz;
    }
}
