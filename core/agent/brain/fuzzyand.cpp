#include "fuzzyand.h"

FuzzyAnd::FuzzyAnd( quint32 id, Brain *brain, QString name, Mode mode) : FuzzyBase(FuzzyBase::AND, brain, id, name, 0, 1), m_mode(mode)
{
}

void FuzzyAnd::calculate()
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
        setResult(tmpResult);
    } else {
        setResult(0);
    }
}

FuzzyAnd::Mode FuzzyAnd::getMode()
{
    return m_mode;
}

void FuzzyAnd::setMode(Mode mode)
{
    m_mode=mode;
}
