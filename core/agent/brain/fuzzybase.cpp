#include "fuzzybase.h"

FuzzyBase::FuzzyBase(LogicType logicType, quint32 id) :
    QObject()
{
    m_logicType=logicType;
    m_id=id;
    m_maxValue=1.0;
    m_minValue=0.0;
    m_result=0.0;
}

void FuzzyBase::inputChanged(InputResult input)
{

}

