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

void FuzzyBase::addChild(FuzzyBase *child)
{
    m_children.append(child);
}

void FuzzyBase::addParent(FuzzyBase *parent, bool isInverted)
{
    Parent par;
    par.parent=parent;
    par.inverted=isInverted;
    m_parents.append(par);
}

QList<FuzzyBase *> FuzzyBase::getChildren()
{
    return m_children;
}

qreal FuzzyBase::getResult(bool inverted) {
    if(!inverted)
        return m_result;
    else
        return m_maxValue-m_result;
}

void FuzzyBase::inputChanged()
{
    this->calculate();
}

bool FuzzyBase::hasChildren()
{
    if(m_children.count()>0) {
        return true;
    } else
        return false;

}

bool FuzzyBase::hasParents()
{
    if(m_parents.count()>0) {
        return true;
    } else
        return false;

}

/** \brief checks if connection to a given parent id is inverted
        \param  parentId the id of the parent to check
        @returns true if connection is inverted
**/
bool FuzzyBase::isConnectionInverted(quint32 parentId)
{
    foreach(Parent parent, m_parents) {
        if(parent.parent->getId()==parentId) {
            return parent.inverted;
        }
    }
    qDebug() << __PRETTY_FUNCTION__ << "Fuzzy "<< m_id << "has no parent with id" << parentId;
    return false;
}

void FuzzyBase::setMax(qreal max)
{
    m_maxValue=max;
}

void FuzzyBase::setMin(qreal min)
{
    m_minValue=min;
}

/** \brief sets the result of this node

                sets the result

        \param  result the result
        @returns true if result was changed
**/
bool FuzzyBase::setResult(qreal result)
{
    result=qBound(m_minValue,result,m_maxValue);
    if(m_result!=result) {
        m_result=result;
        emit resultChanged();
        return true;
    }
    else {
        return false;
    }
}
