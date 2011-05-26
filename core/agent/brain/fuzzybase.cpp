#include "fuzzybase.h"

/** \brief  Baseclass of logic elements

        This class is the base of all logical elemets (and, orr, fuzz, defuzz, input, output, ...)

        It handles the forwarding of changes of results via Qt´s Signal/Slot feature
        and provides low-level functionality

**/
FuzzyBase::FuzzyBase(LogicType logicType, quint32 id, QString name, quint32 editorX, quint32 editorY) :
    QObject()
{
    m_logicType=logicType;
    m_id=id;
    m_maxValue=1.0;
    m_minValue=0.0;
    m_result=0.0;
    m_name=name;
    m_editX=editorX;
    m_editY=editorY;
}

void FuzzyBase::inputChanged(InputResult input)
{

}

