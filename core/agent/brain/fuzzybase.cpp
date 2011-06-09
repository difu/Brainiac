#include "fuzzybase.h"

#include "core/agent/brain/brain.h"

/** \brief  Baseclass of logic elements

        This class is the base of all logical elemets (and, orr, fuzz, defuzz, input, output, ...)

        It handles the forwarding of changes of results via Qt´s Signal/Slot feature
        and provides low-level functionality

**/
FuzzyBase::FuzzyBase(LogicType logicType, Brain *brain, quint32 id, QString name, qreal min=0.0f, qreal max=1.0f) :
    QObject()
{
    m_brain=brain;
    m_logicType=logicType;
    m_id=id;
    m_maxValue=max;
    m_minValue=min;
    m_result=0.0;
    m_name=name;
}

void FuzzyBase::inputChanged()
{

}

void FuzzyBase::setMax(qreal max)
{
    m_maxValue=max;
}

void FuzzyBase::setMin(qreal min)
{
    m_minValue=min;
}

void FuzzyBase::setResult(qreal result)
{
    if(m_result!=result) {
        emit resultChanged();
        m_result=qBound(m_minValue,result,m_maxValue);
    }
}
