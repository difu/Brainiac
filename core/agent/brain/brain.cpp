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
            } else {
                qCritical() <<  __PRETTY_FUNCTION__ << "missing fuzz type" << fuzz->getId();
            }
        }
        // Now we have all fuzzies, let´s connect them
        foreach(FuzzyBase *fuzz,brain->getFuzzies()) {
            if(fuzz->hasChildren()) {
                foreach(FuzzyBase *childFuzz,fuzz->getChildren()) {
                    connectFuzzies(childFuzz->getId(),fuzz->getId(),false); //!< @todo Handle innverted!
                }
            }
        }
    }
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
            @param childId the fuzz to receive the output/result
            @param parentId the fuzz to send it´s result
**/
void Brain::connectFuzzies(quint32 childId, quint32 parentId, bool inverted)
{
    FuzzyBase *child=getFuzzy(childId);
    FuzzyBase *parent=getFuzzy(parentId);
    child->addParent(parent);
    parent->addChild(child);
    connect(parent,SIGNAL(resultChanged()), child, SLOT(inputChanged()),Qt::DirectConnection);
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
