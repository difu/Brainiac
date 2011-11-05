#include "fuzzyand.h"
#include "fuzzyfuzz.h"
#include "input.h"
#include "core/agent/brain/brain.h"
#include "core/agent/agent.h"
#include "core/scene.h"
#include "core/agent/channel.h"
#include "core/simulation.h"

FuzzyAnd::FuzzyAnd( quint32 id, Brain *brain, QString name, Mode mode) : FuzzyBase(FuzzyBase::AND, brain, id, name, 0, 1), m_mode(mode)
{
    m_isSoundRule=false;
}

void FuzzyAnd::calculate()
{
    // Only calculate result, if this is not a sound rule or the simulation is not running (for helping debugging in the braineditor)
    if(!m_isSoundRule || (m_isSoundRule && !m_brain->getAgent()->getScene()->getSimulation()->isRunning())) {
        setResult( calculateFuzzyResult() );
    }
}

qreal FuzzyAnd::calculateFuzzyResult()
{
    if( m_parents.count()>0) {
        qreal tmpResult=m_maxValue;
        foreach(Parent parent, m_parents) {
            qreal parentResult=parent.parent->getResult(parent.inverted);
            switch(m_mode) {
            case MIN:
                if(parentResult<tmpResult)
                    tmpResult=parentResult;
                break;
            case PRODUCT:
                tmpResult*=parentResult;
                break;
            }
        }
        return tmpResult;
    } else {
        return 0;
    }
}

void FuzzyAnd::calculateSound()
{
    Q_ASSERT(m_parents.count()>0 && m_isSoundRule); // there must be at least one fuzzy node as a parent
    Agent *thisAgent=m_brain->getAgent();
    Agent *bestAgent=0;
    qreal bestMatch=0;
    foreach(Agent *otherAgent, thisAgent->getScene()->getAgents()) {
        if(otherAgent==thisAgent) {
            continue;
        }
        //qDebug() << __PRETTY_FUNCTION__ << "other agent "<< otherAgent->getId();
        // qreal distance=thisAgent->getOtherAgentRelativePosition(otherAgent).length();

        //qreal otherAgentAmplitude=otherAgent->getOutputChannel(BrainiacGlobals::ChannelName_Sound_a)->getValue();
        qreal reception=thisAgent->getOtherAgentSoundReception(otherAgent);
        //qDebug() << "Distance  to other "<< distance<<" reception" << reception << "agent "<< otherAgent->getId();
        if(reception<=0.0f) { // we cannot hear the other Agent
            continue;
        }
        foreach(Parent parent, m_parents) {
            Q_ASSERT(parent.parent->getType()==FuzzyBase::FUZZ);
            Q_ASSERT(parent.parent->getParents().count()==1); // there must be only 1 parent which is an input node
            //FuzzyFuzz *fuzz=parent.parent;
            FuzzyBase *fuzzy=parent.parent->getParents().at(0);
            if(fuzzy->getType()==FuzzyBase::INPUT) {
                // Check input channel type of this agent (sound.x, sound.f etc.)
                Input *input=(Input *)fuzzy;
                if(input->isSoundInput()) {
                    if(QString::compare(input->getChannelName(),BrainiacGlobals::ChannelName_Sound_x,Qt::CaseInsensitive)==0 ) {
                        qreal angle=thisAgent->getOtherAgentRelativeAngle(otherAgent);
                       input->setResult(angle,false); // do not emit change, it would result in infinite loop!
                    } else if(QString::compare(input->getChannelName(),BrainiacGlobals::ChannelName_Sound_f,Qt::CaseInsensitive)==0 ) {
                       input->setResult(otherAgent->getOutputChannel(BrainiacGlobals::ChannelName_Sound_f)->getValue(),false); // do not emit change, it would result in infinite loop!
                    } else if(QString::compare(input->getChannelName(),BrainiacGlobals::ChannelName_Sound_d,Qt::CaseInsensitive)==0 ) {
                       qreal tmpReception=thisAgent->getOtherAgentSoundReception(otherAgent)/otherAgent->getOutputChannel(BrainiacGlobals::ChannelName_Sound_a)->getValue();
                       input->setResult(tmpReception,false); // do not emit change, it would result in infinite loop!
                    } else if(QString::compare(input->getChannelName(),BrainiacGlobals::ChannelName_Sound_ox,Qt::CaseInsensitive)==0 ) {
                       input->setResult(thisAgent->getOtherAgentRelativeOrientation(otherAgent),false); // do not emit change, it would result in infinite loop!
                    }
                } else {
                    qWarning() << __PRETTY_FUNCTION__ << "Input" << input->getId() << "is not a sound input "<< input->getName() << input->getChannelName();
                }
            } else {
                qCritical() << __PRETTY_FUNCTION__ << "Expected Input node! ";
            }
            FuzzyFuzz* fuzz=(FuzzyFuzz*) parent.parent;
            fuzz->calculate(false);
        }
        qreal tmpResult=calculateFuzzyResult();
        //qDebug() << __PRETTY_FUNCTION__ << "tmp match agent "<< otherAgent->getId() << tmpResult;
        if(bestMatch<tmpResult) {
            bestAgent=otherAgent;
            bestMatch=tmpResult;
            // qDebug() << __PRETTY_FUNCTION__ << "new best agent "<< otherAgent->getId() << bestMatch;
        }
    }
    setResult(bestMatch);
}

FuzzyAnd::Mode FuzzyAnd::getMode() const
{
    return m_mode;
}

void FuzzyAnd::setMode(Mode mode)
{
    m_mode=mode;
}

void FuzzyAnd::setIsSoundRule(bool isSoundRule)
{
    m_isSoundRule=isSoundRule;
}
