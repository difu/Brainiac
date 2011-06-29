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
    qreal getDefuzzVal();
    qreal getDefuzzValAbs();
    bool isElse();
    void setDefuzzVal(qreal val);
    void setDefuzzValAbs(qreal val);
    void setElse(bool isElse);

protected:
    qreal m_defuzzVal;
    bool m_isElse;
};

#endif // FUZZYDEFUZZ_H
