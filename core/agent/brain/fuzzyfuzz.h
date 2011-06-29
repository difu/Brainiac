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

protected:
    Mode m_mode;
    InterpolationMode m_interpolationMode;
    qreal m_p1, m_p2, m_p3, m_p4;
    qreal getFuzzOut( qreal val );
    qreal getFuzzOut( qreal min, qreal max, qreal val );
    Mode getMode();
    qreal getParentMin();
    qreal getParentMax();
    InterpolationMode getInterpolationMode();
    void setInterpolationMode(InterpolationMode mode);
    void setMode(Mode mode);
    void setP1(qreal p1);
    void setP2(qreal p2);
    void setP3(qreal p3);
    void setP4(qreal p4);

    QEasingCurve m_easing;
};

#endif // FUZZYFUZZ_H
