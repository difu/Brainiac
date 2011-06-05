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
    enum FuzzType{DIRAC,ACTIVATE,DEACTIVATE,TRIANGLE,TRAPEZOID};
    explicit FuzzyBase(LogicType logicType, Brain *brain, quint32 id, QString name);
    quint32 getId() { return m_id; }
    qreal getMinValue() {return m_minValue;}
    qreal getMaxValue() {return m_maxValue;}
    QString getName() {return m_name;}
    qreal getResult() { return m_result; }
    LogicType getType() {return m_logicType; }
    void setId(quint32 id) { m_id=id; }
    void setMax(qreal max);
    void setMin(qreal min);
    void setName( QString name );
    virtual void setResult(qreal result)=0;

protected:
    Brain *m_brain;
    LogicType m_logicType;
    virtual void calculate()=0;
    qreal m_maxValue;
    qreal m_minValue;
    qreal m_result;
    quint32 m_id;

    struct InputResult {
        quint32 source;
        QObject *sourceObject;
        qreal value;
    };
    QString m_name;
signals:
    void resultChanged(InputResult result); //!< This signal is emitted whenever result changes
    void invertedResultChanged(InputResult result); //!< This signal is emitted whenever inverted result changes
public slots:
    void inputChanged(InputResult input);

};

#endif // FUZZYBASE_H
