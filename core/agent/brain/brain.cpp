#include "brain.h"
#include "core/agent/agent.h"
#include "core/agent/brain/output.h"

Brain::Brain(Agent *agent) :
    QObject(),m_agent(agent)
{

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
void Brain::addOutputFuzz(quint32 id, QString name, QString channel, quint32 editorX, quint32 editorY)
{
    Output *out=new Output(id, this, name, channel, editorX, editorY);
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
