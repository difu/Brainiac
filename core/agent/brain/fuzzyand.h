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


#ifndef FUZZYAND_H
#define FUZZYAND_H

#include "core/agent/brain/fuzzybase.h"

/** \brief And fuzz node
**/
class FuzzyAnd : public FuzzyBase
{
public:
    enum Mode { MIN,        //!< take the minimum input as result
                PRODUCT     //!< take the product of all inputs as result
              };
    FuzzyAnd( quint32 id, Brain *brain, QString name, Mode mode);
    /** \brief triggers calculation of result

            Calculates the result depending on the mode
            Nothing is calculated here, if this is a sound rule node.
            This is to prevent further signalling changed results and wrong input, as the correct inputs are determined by this node
            Use calculateSound() instead.
            @sa Agent::advance()
    **/
    void calculate();

    /** \brief triggers calculation of result

            Calculates the result depending on the mode
            Nothing is calculated here, if this is a sound rule node.
            If so, calculation is triggered by calling Agent::advance()
            @sa Agent::advance()
    **/
    void calculateSound();
    bool isSoundRule() const { return m_isSoundRule; }
    Mode getMode() const;

    /**
     * @brief sets the Mode og this FuzzyAnd
     * @sa FuzzyAnd::Mode
     * @param mode the Mode of this FuzzyAnd rule
     */
    void setMode(Mode mode);
    void setIsSoundRule( bool isSoundRule );

protected:
    /**
     * @brief calculates the result of this FuzzyAnd rule and returns it
     *
     * The result is calculated but not written to this fuzz´s internal result.
     * there is also no signal emmitted
     * @return qreal result of this FuzzyAnd rule
     */
    qreal calculateFuzzyResult();
    Mode m_mode; /**< the mode of this FuzzyAnd */
    bool m_isSoundRule;
};

#endif // FUZZYAND_H
