#ifndef FUZZYAND_H
#define FUZZYAND_H

#include "core/agent/brain/fuzzybase.h"

class FuzzyAnd : public FuzzyBase
{
public:
    enum Mode { MIN,        //!< take the minimum input as result
                PRODUCT     //!< take the product of all inputs as result
              };
    FuzzyAnd( quint32 id, Brain *brain, QString name, Mode mode);
    void calculate();
    Mode getMode();
    void setMode(Mode mode);

protected:
    Mode m_mode;
};

#endif // FUZZYAND_H
