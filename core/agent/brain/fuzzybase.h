#ifndef FUZZYBASE_H
#define FUZZYBASE_H

#include <QObject>
#include "core/brainiacglobals.h"

class FuzzyBase : public QObject
{
    Q_OBJECT
public:
    enum LogicType{AND=BrainiacGlobals::AND,OR=BrainiacGlobals::OR,OUTPUT=BrainiacGlobals::OUTPUT,INPUT=BrainiacGlobals::INPUT,DEFUZZ=BrainiacGlobals::DEFUZZ,FUZZ=BrainiacGlobals::FUZZ,NOISE=BrainiacGlobals::NOISE,TIMER=BrainiacGlobals::TIMER};
    enum FuzzType{DIRAC,ACTIVATE,DEACTIVATE,TRIANGLE,TRAPEZOID};
    explicit FuzzyBase(LogicType logicType, quint32 id, QString name, quint32 editorX, quint32 editorY);
    qint32 getEditorTranslationX() { return m_editX; }
    qint32 getEditorTranslationY() { return m_editY; }
    QString & getName() {return m_name;}
    void setId(quint32 id) { m_id=id; }
    void setName( QString name );

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
    quint32 m_editX, m_editY;
    QString m_name;
signals:
    void resultChanged(InputResult result); //!< This signal is emitted whenever result changes
    void invertedResultChanged(InputResult result); //!< This signal is emitted whenever inverted result changes
public slots:
    void inputChanged(InputResult input);

};

#endif // FUZZYBASE_H
