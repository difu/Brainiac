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
    /** \brief  constructs a noise fuzz node

            @param id the id of the noise node
            @param *brain pointer to the brain this noise belongs to
            @param name the name of this noise node
            @param rate the rate of this noise node

    **/
    Noise(quint32 id, Brain *brain, QString name, qreal rate);
    /** \brief returns the current rate
                    @returns rate of this noise fuzz
    **/
    qreal getRate();

    /** \brief returns the max rate for any noise fuzz node

                    currently set fix to 100
                    @returns 100.0f
    **/
    const static qreal getMaxRate() {return 100.0f;}

    /** \brief sets the rate of this agent
        the rate is a time based value in which the value changes between to pseudo random numbers. \n
        A rate of 0 returns a fixed pseudo random number
    **/
    void setRate(qreal rate);

    /** \brief calculates and interpolates the noise
    **/
    void calculate();


protected:
    qreal m_rate; //!< the rate of this noise node
    quint32 m_pseudoSalt; //!< pseudo salt to create "unique" random values
};

#endif // NOISE_H
