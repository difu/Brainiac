#ifndef FUZZYOR_H
#define FUZZYOR_H

#include "core/agent/brain/fuzzybase.h"

class FuzzyOr : public FuzzyBase
{
public:
    FuzzyOr( quint32 id, Brain *brain, QString name);
};

#endif // FUZZYOR_H
