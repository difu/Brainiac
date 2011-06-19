#include "fuzzyand.h"

FuzzyAnd::FuzzyAnd( quint32 id, Brain *brain, QString name) : FuzzyBase(FuzzyBase::OR, brain, id, name, 0, 1)
{
}
