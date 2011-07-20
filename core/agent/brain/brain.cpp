#include "brain.h"
#include "core/agent/agent.h"
#include "core/agent/brain/input.h"
#include "core/agent/brain/output.h"
#include "core/agent/brain/noise.h"
#include "core/agent/brain/fuzzydefuzz.h"
#include "core/agent/brain/fuzzyfuzz.h"

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
            } else if(fuzz->getType()==FuzzyBase::DEFUZZ) {
                FuzzyDefuzz *origDefuzz=(FuzzyDefuzz *)fuzz;
                addDefuzz(origDefuzz->getId(),origDefuzz->getName(),origDefuzz->getDefuzzVal(),origDefuzz->isElse());
            } else if(fuzz->getType()==FuzzyBase::FUZZ) {
                FuzzyFuzz *origFuzz=(FuzzyFuzz *)fuzz;
                addFuzzFuzz(origFuzz->getId(),origFuzz->getName(),origFuzz->getMode(),origFuzz->getInterpolationMode(),origFuzz->getP1(),origFuzz->getP2(),origFuzz->getP3(),origFuzz->getP4());
            } else if(fuzz->getType()==FuzzyBase::TIMER) {
                Timer *origTimer=(Timer *)fuzz;
                addTimerFuzz(origTimer->getId(),origTimer->getName(),origTimer->getRate(),origTimer->getMode());

            }  else {
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

void Brain::addAndFuzz(FuzzyAnd *andFuzz)
{
    m_fuzzies.append(andFuzz);
}

void Brain::addAndFuzz(quint32 id, QString name, FuzzyAnd::Mode mode)
{
    FuzzyAnd *newAnd=new FuzzyAnd(id, this, name, mode);
    addAndFuzz(newAnd);
}

void Brain::addDefuzz(FuzzyDefuzz *defuzz)
{
    m_fuzzies.append(defuzz);
}

void Brain::addDefuzz(quint32 id, QString name, qreal defuzzValue, bool isElse)
{
    FuzzyDefuzz *defuzz=new FuzzyDefuzz(id, this, name, defuzzValue);
    defuzz->setElse(isElse);
    addDefuzz(defuzz);
}

void Brain::addFuzzFuzz(quint32 id, QString name, FuzzyFuzz::Mode mode, FuzzyFuzz::InterpolationMode intMode, qreal p1, qreal p2, qreal p3, qreal p4)
{
    FuzzyFuzz *fuzz=new FuzzyFuzz(id, this, name, mode, intMode);
    fuzz->setP1(p1);
    fuzz->setP2(p2);
    fuzz->setP3(p3);
    fuzz->setP4(p4);
    addFuzzFuzz(fuzz);
}

void Brain::addFuzzFuzz(FuzzyFuzz *fuzzyFuzz)
{
    m_fuzzies.append(fuzzyFuzz);
}

void Brain::addInputFuzz(Input *input)
{
    m_fuzzies.append(input);
}

void Brain::addInputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max)
{
    Input *input=new Input(id, this, name, channel, min, max);
    addInputFuzz(input);
}

void Brain::addOrFuzz(FuzzyOr *orFuzz)
{
    m_fuzzies.append(orFuzz);
}

void Brain::addOrFuzz(quint32 id, QString name, FuzzyOr::Mode mode)
{
    FuzzyOr *newOr=new FuzzyOr(id, this, name, mode);
    addOrFuzz(newOr);
}

void Brain::addOutputFuzz(Output *out)
{
    m_fuzzies.append(out);
}

void Brain::addOutputFuzz(quint32 id, QString name, QString channel, qreal min, qreal max)
{
    Output *out=new Output(id, this, name, channel, min, max);
    addOutputFuzz(out);
}

void Brain::addNoiseFuzz(Noise *noise)
{
    m_fuzzies.append(noise);
}

void Brain::addNoiseFuzz(quint32 id, QString name, qreal rate)
{
    Noise *noise=new Noise(id, this, name, rate);
    addNoiseFuzz(noise);
}

void Brain::addTimerFuzz(Timer *timer)
{
    m_fuzzies.append(timer);
}

void Brain::addTimerFuzz(quint32 id, QString name, qreal rate, Timer::TimerMode mode)
{
    Timer *timer=new Timer(id,this,name,rate,mode);
    addTimerFuzz(timer);
}

void Brain::connectFuzzies(quint32 childId, quint32 parentId, bool inverted)
{
    FuzzyBase *child=getFuzzy(childId);
    FuzzyBase *parent=getFuzzy(parentId);
    if(child && parent) {
        child->addParent(parent,inverted);
        parent->addChild(child);
        connect(parent,SIGNAL(resultChanged()), child, SLOT(inputChanged()),Qt::DirectConnection);
        child->calculate();
    } else {
        qDebug() << __PRETTY_FUNCTION__ << "Whoops! Source or dest are invalid! " << child << parent;
        qDebug() << __PRETTY_FUNCTION__ << m_agent->getId() << m_agent->getPosition()->x() << m_agent->getPosition()->z();
    }
}

Agent* Brain::getAgent()
{
    return m_agent;
}

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
