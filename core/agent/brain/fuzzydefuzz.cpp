#include "fuzzydefuzz.h"
#include "output.h"

FuzzyDefuzz::FuzzyDefuzz(quint32 id, Brain *brain, QString name, qreal defuzzVal) :
    FuzzyBase(FuzzyBase::DEFUZZ, brain, id, name, 0.0, 1.0)
{
    setDefuzzVal(defuzzVal);
    m_isElse=false;
}

void FuzzyDefuzz::calculate()
{
    if(m_isElse) {
        qreal result=1;
        foreach(FuzzyBase *fuzz, m_children) {
            if(fuzz->getType()==OUTPUT) {
                foreach(FuzzyBase *fuzzyParent,fuzz->getParents()) {
                    if(fuzzyParent->getType()==DEFUZZ) {
                        if(fuzzyParent==this)
                            continue;
                        else {
                            FuzzyDefuzz *defuzz=(FuzzyDefuzz*)fuzzyParent;
                            result-=defuzz->getResult();
                        }
                    }
                }
            }
        }
        setResult(result);
    } else {
        if(m_parents.count()>0) {
            Parent par=m_parents.at(0);
            setResult(par.parent->getResult(par.inverted));
        }
    // Find else defuzz
        foreach(FuzzyBase *fuzz, m_children) {
            if(fuzz->getType()==OUTPUT) {
                foreach(FuzzyBase *fuzzyParent,fuzz->getParents()) {
                    if(fuzzyParent->getType()==DEFUZZ) {
                        if(fuzzyParent==this)
                            continue;
                        else {
                            FuzzyDefuzz *defuzz=(FuzzyDefuzz*)fuzzyParent;
                            if(defuzz->isElse())
                                defuzz->calculate();
                        }
                    }
                }
            }
        }
    }
}

qreal FuzzyDefuzz::getDefuzzVal()
{
    return m_defuzzVal;
}

qreal FuzzyDefuzz::getDefuzzValAbs()
{
    if(m_children.count()>0) {
        FuzzyBase *fuzz=m_children.at(0); // only check the first connected fuzz
        if(fuzz->getType()==FuzzyBase::OUTPUT) {
            Output *out=(Output *)fuzz;
            qreal min=out->getMinValue();
            qreal max=out->getMaxValue();
            return BrainiacGlobals::deNorm(min,max,m_defuzzVal);
        }
    }
    return m_defuzzVal;

}

bool FuzzyDefuzz::isElse()
{
    return m_isElse;
}

void FuzzyDefuzz::setDefuzzVal(qreal val)
{
    m_defuzzVal=qBound(0.0,val,1.0);
}

void FuzzyDefuzz::setDefuzzValAbs(qreal val)
{
    if(m_children.count()>0) {
        FuzzyBase *fuzz=m_children.at(0); // only check the first connected fuzz
        if(fuzz->getType()==FuzzyBase::OUTPUT) {
            Output *out=(Output *)fuzz;
            qreal min=out->getMinValue();
            qreal max=out->getMaxValue();
            setDefuzzVal(BrainiacGlobals::norm(min,max,val));
        }
    }
}

void FuzzyDefuzz::setElse(bool isElse)
{
    m_isElse=isElse;
}
