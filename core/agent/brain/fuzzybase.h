#ifndef FUZZYBASE_H
#define FUZZYBASE_H

#include <QObject>
#include "core/brainiacglobals.h"

class Brain;

class FuzzyBase : public QObject
{
    Q_OBJECT
public:
    enum LogicType{AND=BrainiacGlobals::AND,OR=BrainiacGlobals::OR,OUTPUT=BrainiacGlobals::OUTPUT,INPUT=BrainiacGlobals::INPUT,DEFUZZ=BrainiacGlobals::DEFUZZ,FUZZ=BrainiacGlobals::FUZZ,NOISE=BrainiacGlobals::NOISE,TIMER=BrainiacGlobals::TIMER};
    explicit FuzzyBase(LogicType logicType, Brain *brain, quint32 id, QString name, qreal min, qreal max);
    void addChild(FuzzyBase *child);
    void addParent(FuzzyBase *parent, bool isInverted=false);
    virtual void calculate()=0;
    QList<FuzzyBase *> getChildren();
    quint32 getId() { return m_id; }
    qreal getMinValue() {return m_minValue;}
    qreal getMaxValue() {return m_maxValue;}
    QString getName() {return m_name;}
    qreal getResult( bool inverted=false );
    LogicType getType() {return m_logicType; }
    bool hasChildren();
    bool hasParents();
    bool isConnectionInverted( quint32 parentId );
    void setId(quint32 id) { m_id=id; }
    void setMax(qreal max);
    void setMin(qreal min);
    void setName( QString name );
    virtual bool setResult(qreal result);

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
