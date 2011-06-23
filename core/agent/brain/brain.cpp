#include "brain.h"
#include "core/agent/agent.h"
#include "core/agent/brain/input.h"
#include "core/agent/brain/output.h"
#include "core/agent/brain/noise.h"

Brain::Brain(Agent *agent, Brain *brain) :
    QObject(),m_agent(agent)
{
    if(brain) {
        foreach(FuzzyBase *fuzz,brain->getFuzzies()) {
            if(fuzz->getType()==FuzzyBase::OUTPUT) {
                Output *origOut=(Output *)fuzz;
                addOutputFuzz(origOut->getId(),origOut->getName(),origOut->getChannelName(),origOut->getMinValue(),origOut->getMaxValue());
            } else if(fuzz->getType()==FuzzyBase::INPUT) {
                Input *origInput=(Input *)fuzz;
                addInputFuzz(origInput->getId(),origInput->getName(),origInput->getChannelName(),origInput->getMinValue(),origInput->getMaxValue());
            } else if(fuzz->getType()==FuzzyBase::NOISE) {
                Noise *origNoise=(Noise *)fuzz;
                addNoiseFuzz(origNoise->getId(),origNoise->getName(),origNoise->getRate());
            } else if(fuzz->getType()==FuzzyBase::AND) {
                FuzzyAnd *origAnd=(FuzzyAnd *)fuzz;
                addAndFuzz(origAnd->getId(),origAnd->getName(),origAnd->getMode());
            } else if(fuzz->getType()==FuzzyBase::OR) {
                FuzzyOr *origOr=(FuzzyOr *)fuzz;
                addOrFuzz(origOr->getId(),origOr->getName(),origOr->getMode());
            } else {
                qCritical() <<  __PRETTY_FUNCTION__ << "missing fuzz type" << fuzz->getId();
            }
        }
        // Now we have all fuzzies, let´s connect them
        foreach(FuzzyBase *fuzz,brain->getFuzzies()) {
            if(fuzz->hasChildren()) {
                foreach(FuzzyBase *childFuzz,fuzz->getChildren()) {
                    connectFuzzies(childFuzz->getId(),fuzz->getId(),childFuzz->isConnectionInverted(fuzz->getId()));
                }
            }
        }
    }
}

/** \brief adds an and fuzzy rule to this brain
            @param the and rule to be added
**/
void Brain::addAndFuzz(FuzzyAnd *andFuzz)
{
    m_fuzzies.append(andFuzz);
}

/** \brief adds an and fuzzy rule to this brain
**/
void Brain::addAndFuzz(quint32 id, QString name, FuzzyAnd::Mode mode)
{
    FuzzyAnd *newAnd=new FuzzyAnd(id, this, name, mode);
    addAndFuzz(newAnd);
}

/** \brief adds an input to this brain
            @param the input to be added
**/
void Brain::addInputFuzz(Input *input)
{
    m_fuzzies.append(input);
}

/** \brief adds an input to this brain
**/
void Brain::addInputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max)
{
    Input *input=new Input(id, this, name, channel, min, max);
    addInputFuzz(input);
}

/** \brief adds an or fuzzy rule to this brain
            @param the or rule to be added
**/
void Brain::addOrFuzz(FuzzyOr *orFuzz)
{
    m_fuzzies.append(orFuzz);
}

/** \brief adds an or fuzzy rule to this brain
**/
void Brain::addOrFuzz(quint32 id, QString name, FuzzyOr::Mode mode)
{
    FuzzyOr *newOr=new FuzzyOr(id, this, name, mode);
    addOrFuzz(newOr);
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
void Brain::addOutputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max)
{
    Output *out=new Output(id, this, name, channel, min, max);
    addOutputFuzz(out);
}

/** \brief adds a noise fuzz to this brain
            @param the noise to be added
**/
void Brain::addNoiseFuzz(Noise *noise)
{
    m_fuzzies.append(noise);
}

/** \brief adds a noise fuzz to this brain
            @param the noise to be added
**/
void Brain::addNoiseFuzz(quint32 id, QString name, qreal rate)
{
    Noise *noise=new Noise(id, this, name, rate);
    addNoiseFuzz(noise);
}

/** \brief connect two fuzz nodes
            Connect a child to a parent. After the fuzzies are connected, a calculation of the child is issued to fetch the value of the parent(s)
            @param childId the fuzz to receive the output/result
            @param parentId the fuzz to send it´s result
            @param inverted if the parent´s result must be inverted
            @sa FuzzyBase
**/
void Brain::connectFuzzies(quint32 childId, quint32 parentId, bool inverted)
{
    FuzzyBase *child=getFuzzy(childId);
    FuzzyBase *parent=getFuzzy(parentId);
    child->addParent(parent,inverted);
    parent->addChild(child);
    connect(parent,SIGNAL(resultChanged()), child, SLOT(inputChanged()),Qt::DirectConnection);
    child->calculate();
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
