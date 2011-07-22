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
    explicit FuzzyBase(LogicType logicType, Brain *brain, quint32 id, QString name, qreal min, qreal max);
    virtual ~FuzzyBase();
    void addChild(FuzzyBase *child);
    void addParent(FuzzyBase *parent, bool isInverted=false);
    virtual void calculate()=0;
    void deleteChild(FuzzyBase *child);
    void deleteParent(FuzzyBase *parent);
    QList<FuzzyBase *> getChildren();
    quint32 getId() { return m_id; }
    qreal getMinValue() {return m_minValue;}
    qreal getMaxValue() {return m_maxValue;}
    QString getName() {return m_name;}
    QList<FuzzyBase *> getParents();
    qreal getResult( bool inverted=false );
    LogicType getType() {return m_logicType; }
    bool hasChildren();
    bool hasParents();
    /** \brief checks if connection to a given parent id is inverted
            \param  parentId the id of the parent to check
            @returns true if connection is inverted
    **/
    bool isConnectionInverted( quint32 parentId );
    void setId(quint32 id) { m_id=id; }
    void setMax(qreal max);
    void setMin(qreal min);
    void setName( QString name );
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
    void inputChanged();

};

#endif // FUZZYBASE_H
