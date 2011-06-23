#include "fuzzyor.h"

FuzzyOr::FuzzyOr( quint32 id, Brain *brain, QString name, Mode mode) : FuzzyBase(FuzzyBase::OR, brain, id, name, 0, 1),m_mode(mode)
{
}

void FuzzyOr::calculate()
{
    if( m_parents.count()>0) {
        qreal tmpResult=m_minValue;
        foreach(Parent parent, m_parents) {
            qreal parentResult=parent.parent->getResult(parent.inverted);
            switch(m_mode) {
            case MAX:
                if(parentResult>tmpResult)
                    tmpResult=parentResult;
                break;
            case SUM:
                tmpResult+=parentResult;
            }
        }
        setResult(tmpResult);
    } else {
        setResult(0);
    }
}

FuzzyOr::Mode FuzzyOr::getMode()
{
    return m_mode;
}

void FuzzyOr::setMode(Mode mode)
{
    m_mode=mode;
}
