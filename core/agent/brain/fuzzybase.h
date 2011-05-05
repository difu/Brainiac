#ifndef FUZZYBASE_H
#define FUZZYBASE_H

#include <QObject>

class FuzzyBase : public QObject
{
    Q_OBJECT
public:
    enum LogicType{AND,OR,OUTPUT,INPUT,DEFUZZ,FUZZ,NOISE,TIMER};
    enum FuzzType{DIRAC,ACTIVATE,DEACTIVATE,TRIANGLE,TRAPEZOID};
    explicit FuzzyBase(LogicType logicType, quint32 id);
    void setId(quint32 id) { m_id=id; }

protected:
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
signals:
    void resultChanged(InputResult result); //!< This signal is emitted whenever result changes
    void invertedResultChanged(InputResult result); //!< This signal is emitted whenever inverted result changes
public slots:
    void inputChanged(InputResult input);

};

#endif // FUZZYBASE_H
