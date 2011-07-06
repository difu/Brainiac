#include "fuzzyfuzz.h"

FuzzyFuzz::FuzzyFuzz( quint32 id, Brain *brain, QString name, Mode mode, InterpolationMode iMode) :
    FuzzyBase(FuzzyBase::FUZZ, brain, id, name, 0.0, 1.0),m_mode(mode)
{
    setInterpolationMode(iMode);
    m_p1=0.3;
    m_p2=0.4;
    m_p3=0.7;
    m_p4=0.8;
}

void FuzzyFuzz::calculate()
{
    if(m_parents.count()>0) {
        Parent par=m_parents.at(0);// Only take the first parent
        qreal max=par.parent->getMaxValue();
        qreal min=par.parent->getMinValue();
        setResult(this->getFuzzOut(min,max,par.parent->getResult(par.inverted)));
    } else {
        setResult(0);
    }
}

FuzzyFuzz::Mode FuzzyFuzz::getMode()
{
    return m_mode;
}

qreal FuzzyFuzz::getParentMax()
{
    if(m_parents.count()>0) {
        Parent par=m_parents.at(0);// Only take the first parent
        return par.parent->getMaxValue();
    } else {
        return 1;
    }
}

qreal FuzzyFuzz::getParentMin()
{
    if(m_parents.count()>0) {
        Parent par=m_parents.at(0);// Only take the first parent
        return par.parent->getMinValue();
    } else {
        return 0;
    }
}

FuzzyFuzz::InterpolationMode FuzzyFuzz::getInterpolationMode()
{
    return m_interpolationMode;
}

qreal FuzzyFuzz::getFuzzOut(qreal val)
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

qreal FuzzyFuzz::getFuzzOut( qreal min, qreal max, qreal val ) {
    qreal normVal=BrainiacGlobals::norm( min, max, val );
    if(normVal<m_p1)
        if(m_mode==DEACTIVATE) {
            return 1;
        } else
            return 0;

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

qreal FuzzyFuzz::getP1()
{
    return m_p1;
}

qreal FuzzyFuzz::getP2()
{
    return m_p2;
}

qreal FuzzyFuzz::getP3()
{
    return m_p3;
}

qreal FuzzyFuzz::getP4()
{
    return m_p4;
}

void FuzzyFuzz::setMode(Mode mode)
{
    m_mode=mode;
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

void FuzzyFuzz::setP1(qreal p1)
{
    m_p1=p1;
}

void FuzzyFuzz::setP2(qreal p2)
{
    m_p2=p2;
}

void FuzzyFuzz::setP3(qreal p3)
{
    m_p3=p3;
}

void FuzzyFuzz::setP4(qreal p4)
{
    m_p4=p4;
}
