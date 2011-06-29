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

}

/** \brief  returns the defuzzyfication value

  @returns the defuzzyfication value
  @sa Output

**/
qreal FuzzyDefuzz::getDefuzzVal()
{
    return m_defuzzVal;
}

/** \brief  returns the absolute defuzzyfication value

  the defuzz value is defined by a relative value whichis calculated by multiplying this value by the range of the connected min/max
  output value. If no Output is connected a value between 0 and 1 is returned

  @returns the defuzzyfication value
  @sa Output

**/
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

/** \brief  returns if this Defuzz is an else node
  @returns true, if this is an else node, false otherwise
**/
bool FuzzyDefuzz::isElse()
{
    return m_isElse;
}

/** \brief  sets the defuzzyfication value

  the defuzz value is defined by a relative value which is calculated by multiplying this value by the range of the connected min/max
  output value

  @param val the relative defuzz value to the min/max output value range
  @sa Output

**/
void FuzzyDefuzz::setDefuzzVal(qreal val)
{
    m_defuzzVal=qBound(0.0,val,1.0);
}

/** \brief  sets the defuzzyfication value

  the given value is internally changed to a relative defuzz value

  @param val the absolute defuzz value to the min/max output value range
  @sa FuzzyDefuzz::setDefuzzVal(qreal val)
  @sa Output

**/
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
