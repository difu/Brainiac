// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


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
    m_rate=qBound((double)0.0f,rate,(double)this->getMaxRate());
}

