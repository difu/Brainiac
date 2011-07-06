#ifndef FUZZYDEFUZZ_H
#define FUZZYDEFUZZ_H

#include "core/agent/brain/fuzzybase.h"

/** \brief  Defuzz brain element

  The FuzzyDefuzz brain element... tbc

**/
class FuzzyDefuzz : public FuzzyBase
{
public:
    FuzzyDefuzz( quint32 id, Brain *brain, QString name, qreal defuzzVal);
    void calculate();

    /** \brief  returns the defuzzyfication value

      @returns the defuzzyfication value
      @sa Output

    **/
    qreal getDefuzzVal();

    /** \brief  returns the absolute defuzzyfication value

      the defuzz value is defined by a relative value whichis calculated by multiplying this value by the range of the connected min/max
      output value. If no Output is connected a value between 0 and 1 is returned

      @returns the defuzzyfication value
      @sa Output

    **/
    qreal getDefuzzValAbs();

    /** \brief  returns if this Defuzz is an else node
      @returns true, if this is an else node, false otherwise
    **/
    bool isElse();

    /** \brief  sets the defuzzyfication value

      the defuzz value is defined by a relative value which is calculated by multiplying this value by the range of the connected min/max
      output value

      @param val the relative defuzz value to the min/max output value range
      @sa Output

    **/
    void setDefuzzVal(qreal val);

    /** \brief  sets the defuzzyfication value

      the given value is internally changed to a relative defuzz value

      @param val the absolute defuzz value to the min/max output value range
      @sa FuzzyDefuzz::setDefuzzVal(qreal val)
      @sa Output

    **/
    void setDefuzzValAbs(qreal val);
    void setElse(bool isElse);

protected:
    qreal m_defuzzVal;
    bool m_isElse;
};

#endif // FUZZYDEFUZZ_H
