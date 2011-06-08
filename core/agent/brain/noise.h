#ifndef NOISE_H
#define NOISE_H

#include "core/agent/brain/fuzzybase.h"
#include <QtCore>
/** \brief  Noise fuzz node

        Creates a pseudo random noise for a given rate

**/

class Noise : public FuzzyBase
{
public:
    Noise(quint32 id, Brain *brain, QString name, qreal rate);
    qreal getRate();
    void setRate(qreal rate);
    void calculate();
    void setResult(qreal result);


protected:
    qreal m_rate;
    quint32 m_pseudoSalt;
};

#endif // NOISE_H
