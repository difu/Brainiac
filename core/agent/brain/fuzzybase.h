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


#ifndef FUZZYBASE_H
#define FUZZYBASE_H

#include <QObject>
#include "core/brainiacglobals.h"

class Brain;

/** \brief  Baseclass of logic elements

        This class is the base of all logical elemets (and, orr, fuzz, defuzz, input, output, ...)

        It handles the forwarding of changes of results via Qt´s Signal/Slot feature
        and provides low-level functionality

**/
class FuzzyBase : public QObject
{
    Q_OBJECT
public:
    enum LogicType{AND=BrainiacGlobals::AND,OR=BrainiacGlobals::OR,OUTPUT=BrainiacGlobals::OUTPUT,INPUT=BrainiacGlobals::INPUT,DEFUZZ=BrainiacGlobals::DEFUZZ,FUZZ=BrainiacGlobals::FUZZ,NOISE=BrainiacGlobals::NOISE,TIMER=BrainiacGlobals::TIMER};
    explicit FuzzyBase(LogicType logicType, Brain *brain, quint32 id, const QString &name, qreal min, qreal max);
    virtual ~FuzzyBase();

    /** \brief adds a child to this fuzz node
            The child is not automatically connected. You must call Brain::connectFuzzies
            @param child the child to add
            @sa Brain::connectFuzzies
    **/
    void addChild(FuzzyBase *child);

    /** \brief adds a parent to this fuzz node
            The parent is not automatically connected. You must call Brain::connectFuzzies
            @param child the parent to add
            @sa Brain::connectFuzzies
    **/
    void addParent(FuzzyBase *parent, bool isInverted=false);

    /** \brief triggers calculation of result
            this function is automatically called when slot FuzzyBase::inputChanged is called \n
            typically this happens when the parent fuzz changed its result value and emits FuzzyBase::resultChanged
            @sa Brain::connectFuzzies
            @sa FuzzyBase::inputChanged
            @sa FuzzyBase::resultChanged
    **/
    virtual void calculate()=0;
    void deleteChild(FuzzyBase *child);
    void deleteParent(FuzzyBase *parent);
    QList<FuzzyBase *> getChildren() const;
    quint32 getId() const { return m_id; }
    qreal getMinValue() const {return m_minValue;}
    qreal getMaxValue() const {return m_maxValue;}
    QString getName() const {return m_name;}
    QList<FuzzyBase *> getParents() const;

    /** \brief returns the calculated result of this fuzz
            @param inverted if true returns the inverted result (max value minus result)
            @returns result the calculated result
    **/
    qreal getResult( bool inverted=false ) const;
    LogicType getType() {return m_logicType; }

    /** \brief checks if this fuzz has children
            @returns true this fuzz has children
    **/
    bool hasChildren() const;

    /** \brief checks if this fuzz has parents
            @returns true this fuzz has parents
    **/
    bool hasParents() const;

    /** \brief checks if connection to a given parent id is inverted
            \param  parentId the id of the parent to check
            @returns true if connection is inverted
    **/
    bool isConnectionInverted( quint32 parentId ) const;
    void setId(quint32 id) { m_id=id; }
    void setMax(qreal max);
    void setMin(qreal min);
    void setName( const QString &name );
    /** \brief sets the result of this node

                    sets the result

            \param  result the result
            @returns true if result was changed
    **/
    virtual bool setResult(qreal result,bool emitChange=true);

protected:
    struct Parent {
        FuzzyBase *parent;
        bool inverted;
    };
    Brain *m_brain;
    LogicType m_logicType;
    qreal m_maxValue;
    qreal m_minValue;
    qreal m_result;
    quint32 m_id;
    QList<Parent> m_parents;
    QList<FuzzyBase *> m_children;



    QString m_name;
signals:
    void resultChanged(); //!< This signal is emitted whenever result changes
public slots:
    /** \brief triggers re-calculation of this fuzz´ result
        usually automatically called when a patent emits the FuzzyBase::resultChanged signal
        @sa Brain::connectFuzzies
        @sa FuzzyBase::resultChanged
    **/
    void inputChanged();
};

#endif // FUZZYBASE_H
