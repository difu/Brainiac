#include "noise.h"
#include "core/agent/brain/brain.h"
#include "core/agent/agent.h"
#include "core/scene.h"

Noise::Noise(quint32 id, Brain *brain, QString name, qreal rate):FuzzyBase(FuzzyBase::NOISE,brain,id,name),m_rate(rate)
{
    m_pseudoSalt=id*(m_brain->getAgent()->getId()+1)*13; // +1 because of master agent which has id =0
    setResult(BrainiacGlobals::getRand(m_pseudoSalt));
}

void Noise::calculate()
{

}

qreal Noise::getRate()
{
    return m_rate;
}

void Noise::setRate(qreal rate)
{
    m_rate=rate;
}

void Noise::setResult(qreal result)
{
    m_result=result;
}
