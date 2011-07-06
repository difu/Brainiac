#ifndef FUZZYFUZZ_H
#define FUZZYFUZZ_H
#include <QEasingCurve>

#include "core/agent/brain/fuzzybase.h"

/** \brief  Fuzzy Rule

**/
class FuzzyFuzz : public FuzzyBase
{
public:
    enum Mode {DIRAC,ACTIVATE,DEACTIVATE,TRIANGLE,TRAPEZOID};
    enum InterpolationMode {LINEAR, SINE, QUAD };
    FuzzyFuzz( quint32 id, Brain *brain, QString name, Mode mode, InterpolationMode iMode);
    void calculate();
    /** \brief  returns the fuzzy value
        if this fuzz is connected to a node, it returns the value with respect to the parent´s bounds
        if not, it sets the bounds to 0 and 1
            @returns the fuzzyfication value
    **/
    qreal getFuzzOut( qreal val );
    qreal getFuzzOut( qreal min, qreal max, qreal val );
    Mode getMode();
    qreal getP1();
    qreal getP2();
    qreal getP3();
    qreal getP4();
    /** \brief  returns this fuzz´s parent´s minimum

            @returns the minimum value of its parent, 0 if it has no parent. This is the default min value of any fuzz
    **/
    qreal getParentMin();
    /** \brief  returns this fuzz´s parent´s max

            @returns the maximum value of its parent, 1 if it has no parent. This is the default max value of any fuzz
    **/
    qreal getParentMax();
    InterpolationMode getInterpolationMode();
    void setInterpolationMode(InterpolationMode mode);
    void setMode(Mode mode);
    void setP1(qreal p1);
    void setP2(qreal p2);
    void setP3(qreal p3);
    void setP4(qreal p4);

protected:
    Mode m_mode;
    InterpolationMode m_interpolationMode;
    qreal m_p1, m_p2, m_p3, m_p4;
    QEasingCurve m_easing;
};

#endif // FUZZYFUZZ_H
