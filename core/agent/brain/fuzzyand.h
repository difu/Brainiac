#ifndef FUZZYAND_H
#define FUZZYAND_H

#include "core/agent/brain/fuzzybase.h"

class FuzzyAnd : public FuzzyBase
{
public:
    FuzzyAnd( quint32 id, Brain *brain, QString name);
};

#endif // FUZZYAND_H
