#include "fuzzybase.h"

#include "core/agent/brain/brain.h"

/** \brief  Baseclass of logic elements

        This class is the base of all logical elemets (and, orr, fuzz, defuzz, input, output, ...)

        It handles the forwarding of changes of results via Qt´s Signal/Slot feature
        and provides low-level functionality

**/
FuzzyBase::FuzzyBase(LogicType logicType, Brain *brain, quint32 id, QString name) :
    QObject()
{
    m_brain=brain;
    m_logicType=logicType;
    m_id=id;
    m_maxValue=1.0;
    m_minValue=0.0;
    m_result=0.0;
    m_name=name;
}

void FuzzyBase::inputChanged(InputResult input)
{

}

void FuzzyBase::setMax(qreal max)
{
    m_minValue=max;
}

void FuzzyBase::setMin(qreal min)
{
    m_maxValue=min;
}
