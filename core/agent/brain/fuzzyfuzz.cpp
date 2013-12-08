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


#include "fuzzyfuzz.h"

FuzzyFuzz::FuzzyFuzz( quint32 id, Brain *brain, QString name, Mode mode, InterpolationMode iMode) :
    FuzzyBase(FuzzyBase::FUZZ, brain, id, name, 0.0, 1.0)
{
    setInterpolationMode(iMode);
    setMode(mode);
}

void FuzzyFuzz::calculate()
{
    this->calculate(true);
}

void FuzzyFuzz::calculate(bool emitChange) {
    if(m_parents.count()>0) {
        Parent par=m_parents.at(0);// Only take the first parent
        qreal max=par.parent->getMaxValue();
        qreal min=par.parent->getMinValue();
        setResult(this->getFuzzOut(min,max,par.parent->getResult(par.inverted)),emitChange);
    } else {
        setResult(0,emitChange);
    }
}

FuzzyFuzz::Mode FuzzyFuzz::getMode() const
{
    return m_mode;
}

qreal FuzzyFuzz::getParentMax() const
{
    if(m_parents.count()>0) {
        Parent par=m_parents.at(0);// Only take the first parent
        return par.parent->getMaxValue();
    } else {
        return 1;
    }
}

qreal FuzzyFuzz::getParentMin() const
{
    if(m_parents.count()>0) {
        Parent par=m_parents.at(0);// Only take the first parent
        return par.parent->getMinValue();
    } else {
        return 0;
    }
}

FuzzyFuzz::InterpolationMode FuzzyFuzz::getInterpolationMode() const
{
    return m_interpolationMode;
}

qreal FuzzyFuzz::getFuzzOut(qreal val) const
{
    if(m_parents.count()>0) {
        Parent par=m_parents.at(0);// Only take the first parent
        qreal max=par.parent->getMaxValue();
        qreal min=par.parent->getMinValue();
        return this->getFuzzOut(min,max,val);
    } else {
        return this->getFuzzOut(0,1,val);
    }

}

qreal FuzzyFuzz::getFuzzOut( qreal min, qreal max, qreal val ) const
{
    qreal normVal=BrainiacGlobals::norm( min, max, val );
    if(m_mode==DIRAC) {
        if(m_p1==normVal) { //!< @todo maybe use some kind of fuzzy compare?
            return 1;
        }
        else
        {
            return 0;
        }
    }
    if(normVal<m_p1)
        if(m_mode==DEACTIVATE) {
            return 1;
        } else {
            return 0;
        }

    if(normVal<m_p2) {
        qreal p1p2norm=BrainiacGlobals::norm(m_p1,m_p2,normVal);
        if(m_mode==DEACTIVATE) {
            return 1-m_easing.valueForProgress(p1p2norm);
        }
        return m_easing.valueForProgress(p1p2norm);
    }
    if(m_mode==ACTIVATE) {
        return 1;
    } else {
        if(m_mode==DEACTIVATE) {
            return 0;
        }
    }
    // Activate and Deactivate are not anymore handled
    // do we have to handle a triangle?
    if(m_mode==TRIANGLE) {
        if(normVal<m_p3) {
            qreal p2p3norm=BrainiacGlobals::norm(m_p2,m_p3,normVal);
            return 1-m_easing.valueForProgress(p2p3norm);
        } else {
            return 0;
        }
    }
    Q_ASSERT(m_mode==TRAPEZOID);
    // Trapezoid
    if(normVal<m_p3)
        return 1;
    if(normVal<m_p4) {
        qreal p3p4norm=BrainiacGlobals::norm(m_p3,m_p4,normVal);
        return 1-m_easing.valueForProgress(p3p4norm);
    }
    return 0;

}

qreal FuzzyFuzz::getP1() const
{
    return m_p1;
}

qreal FuzzyFuzz::getP2() const
{
    return m_p2;
}

qreal FuzzyFuzz::getP3() const
{
    return m_p3;
}

qreal FuzzyFuzz::getP4() const
{
    return m_p4;
}

qreal FuzzyFuzz::getP1Abs() const
{
    if(m_parents.count()>0) {
        Parent par=m_parents.at(0);// Only take the first parent
        qreal max=par.parent->getMaxValue();
        qreal min=par.parent->getMinValue();
        return BrainiacGlobals::deNorm(min,max,m_p1);
    } else {
        return m_p1;
    }
}

qreal FuzzyFuzz::getP2Abs() const
{
    if(m_parents.count()>0) {
        Parent par=m_parents.at(0);// Only take the first parent
        qreal max=par.parent->getMaxValue();
        qreal min=par.parent->getMinValue();
        return BrainiacGlobals::deNorm(min,max,m_p2);
    } else {
        return m_p2;
    }
}

qreal FuzzyFuzz::getP3Abs() const
{
    if(m_parents.count()>0) {
        Parent par=m_parents.at(0);// Only take the first parent
        qreal max=par.parent->getMaxValue();
        qreal min=par.parent->getMinValue();
        return BrainiacGlobals::deNorm(min,max,m_p3);
    } else {
        return m_p3;
    }
}

qreal FuzzyFuzz::getP4Abs() const
{
    if(m_parents.count()>0) {
        Parent par=m_parents.at(0);// Only take the first parent
        qreal max=par.parent->getMaxValue();
        qreal min=par.parent->getMinValue();
        return BrainiacGlobals::deNorm(min,max,m_p4);
    } else {
        return m_p4;
    }
}

const qreal FuzzyFuzz::MinDistBetweenPoints=0.000001f;

void FuzzyFuzz::setMode(Mode mode)
{
    m_mode=mode;
    switch(m_mode) {
    case DIRAC:
        m_p1=0.5f;
        break;
    case ACTIVATE:
    case DEACTIVATE:
        m_p1=0.3f;
        m_p2=0.7f;
        break;
    case TRIANGLE:
        m_p1=0.3f;
        m_p2=0.5f;
        m_p3=0.7f;
        break;
    case TRAPEZOID:
        m_p1=0.2f;
        m_p2=0.4f;
        m_p3=0.6f;
        m_p4=0.8f;
        break;
    }
}

void FuzzyFuzz::setInterpolationMode(InterpolationMode mode)
{
    m_interpolationMode=mode;
    switch(m_interpolationMode) {
    case LINEAR:
        m_easing.setType(QEasingCurve::Linear);
        break;
    case SINE:
        m_easing.setType(QEasingCurve::InOutSine);
        break;
    case QUAD:
        m_easing.setType(QEasingCurve::InOutQuad);
        break;
    }
}

qreal FuzzyFuzz::setP1(qreal p1,bool check)
{
    if(check) {
        switch(m_mode) {
        case DIRAC:
            m_p1=qBound((qreal)0.0f,p1,(qreal)1.0f);
            break;
        default:
            m_p1=qBound((qreal)0.0f,p1,m_p2-MinDistBetweenPoints);
        }
    } else
        m_p1=p1;

    return m_p1;
}

qreal FuzzyFuzz::setP2(qreal p2,bool check)
{
    if(check) {
        switch(m_mode) {
        case ACTIVATE:
        case DEACTIVATE:
            m_p2=qBound(m_p1+MinDistBetweenPoints,p2,(qreal)1.0f);
            break;
        case TRIANGLE:
        case TRAPEZOID:
            m_p2=qBound(m_p1+MinDistBetweenPoints,p2,m_p3-MinDistBetweenPoints);
            break;
        default:
            m_p2=qBound(m_p1+MinDistBetweenPoints,p2,m_p3-MinDistBetweenPoints);
        }
    } else
        m_p2=p2;
    return m_p2;
}

qreal FuzzyFuzz::setP3(qreal p3,bool check)
{
    if(check) {
        switch(m_mode) {
        case TRIANGLE:
            m_p3=qBound(m_p2+MinDistBetweenPoints,p3,(qreal)1.0f);
            break;
        default:
            m_p3=qBound(m_p2+MinDistBetweenPoints,p3,m_p4-MinDistBetweenPoints);
        }
    } else
            m_p3=p3;
    return m_p3;
}

qreal FuzzyFuzz::setP4(qreal p4,bool check)
{
    if(check) {
        m_p4=qBound(m_p3+MinDistBetweenPoints,p4,(qreal)1.0f);
    } else
        m_p4=p4;
    return m_p4;
}
