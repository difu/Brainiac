// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "fuzzybase.h"

#include "core/agent/brain/brain.h"

FuzzyBase::FuzzyBase(LogicType logicType, Brain *brain, quint32 id, const QString &name, qreal min=0.0f, qreal max=1.0f) :
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

void FuzzyBase::deleteChild(FuzzyBase *child)
{
    if(m_children.contains(child)) {
        m_children.removeAll(child);
    } else {
        qDebug() << __PRETTY_FUNCTION__ << "This fuzz has no such child";
    }
}

void FuzzyBase::deleteParent(FuzzyBase *parent)
{
    bool found=false;
    forever{
        found=false;
        for (int i = 0; i < m_parents.size(); ++i) {
            Parent par=m_parents.at(i);
            if (par.parent ==parent ) {
                m_parents.removeAt(i);
                found=true;
                //qDebug() << __PRETTY_FUNCTION__ << "removed parent" << parent->getName();
                break;
            }
        }
        if(found==false)
            break;
    }
}

QList<FuzzyBase *> FuzzyBase::getChildren() const
{
    return m_children;
}

QList<FuzzyBase *> FuzzyBase::getParents() const
{
    QList<FuzzyBase *> parents;
    foreach(Parent fuzz, m_parents) {
        parents.append(fuzz.parent);
    }
    return parents;
}

qreal FuzzyBase::getResult(bool inverted) const
{
    if(!inverted)
        return m_result;
    else
        return m_maxValue-m_result;
}

void FuzzyBase::inputChanged()
{
    this->calculate();
}

bool FuzzyBase::hasChildren() const
{
    if(m_children.count()>0) {
        return true;
    } else
        return false;

}

bool FuzzyBase::hasParents() const
{
    if(m_parents.count()>0) {
        return true;
    } else
        return false;

}

bool FuzzyBase::isConnectionInverted(quint32 parentId) const
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
    setResult(m_result); // Crop the current result and emit changes
}

void FuzzyBase::setMin(qreal min)
{
    m_minValue=min;
    setResult(m_result); // Crop the current result and emit changes
}

void FuzzyBase::setName(const QString &name)
{
    m_name=name;
}

bool FuzzyBase::setResult(qreal result,bool emitChange)
{
    result=qBound(m_minValue,result,m_maxValue);
    if(m_result!=result) {
        m_result=result;
        if(emitChange)
            emit resultChanged();
        return true;
    }
    else {
        return false;
    }
}

FuzzyBase::~FuzzyBase() {
    foreach(FuzzyBase *child, m_children) {
        child->deleteParent(this);
    }
    foreach(Parent par,m_parents) {
        par.parent->deleteChild(this);
    }
    //qDebug() << __PRETTY_FUNCTION__ << "Fuzzy deleted "<< m_id;
}
