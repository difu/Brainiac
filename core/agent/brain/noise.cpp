#include "noise.h"
#include "core/agent/brain/brain.h"
#include "core/agent/agent.h"
#include "core/scene.h"
#include "core/simulation.h"

Noise::Noise(quint32 id, Brain *brain, QString name, qreal rate):FuzzyBase(FuzzyBase::NOISE,brain,id,name,0,1),m_rate(rate)
{
    m_pseudoSalt=id*(m_brain->getAgent()->getId()+1)*13; // +1 because of master agent which has id =0
    setResult(BrainiacGlobals::getRand(m_pseudoSalt));
}

void Noise::calculate()
{
    if(m_rate) {
        quint32 currentFrame=m_brain->getAgent()->getScene()->getSimulation()->getCurrentFrame();
        quint32 fps=m_brain->getAgent()->getScene()->getSimulation()->getFps();
        quint32 i=1;
        while(fps*m_rate*i<currentFrame) {
            i++;
        }
        quint32 lastRandFrame=(i-1)*(fps*m_rate);
        quint32 nextRandFrame=(i)*(fps*m_rate);
        qreal lastRand=BrainiacGlobals::getRand(m_pseudoSalt+lastRandFrame);
        qreal nextRand=BrainiacGlobals::getRand(m_pseudoSalt+nextRandFrame);
        qreal diff=nextRand-lastRand;
        qreal value=lastRand+(diff*(currentFrame-lastRandFrame)/(fps*m_rate));
        setResult(value);
     }
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
