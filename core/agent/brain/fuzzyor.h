#ifndef FUZZYOR_H
#define FUZZYOR_H

#include "core/agent/brain/fuzzybase.h"

class FuzzyOr : public FuzzyBase
{
public:
    enum Mode { MAX,    //!< take the max input as resutl
                SUM     //!< tale the sum of all inputs as result
              };
    FuzzyOr( quint32 id, Brain *brain, QString name, Mode mode);
    void calculate();
    Mode getMode();
    void setMode(Mode mode);


protected:
    Mode m_mode;
};

#endif // FUZZYOR_H
