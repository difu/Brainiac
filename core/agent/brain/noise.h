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
    Noise(quint32 id, Brain *brain, qreal rate);
    /** \brief returns the current rate
                    @returns rate of this noise fuzz
    **/
    qreal getRate();

    /** \brief returns the max rate for any noise fuzz node

                    currently set fix to 100
                    @returns 100.0f
    **/
    static qreal getMaxRate() {return 100.0f;}

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
