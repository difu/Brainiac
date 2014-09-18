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


#ifndef FUZZYDEFUZZ_H
#define FUZZYDEFUZZ_H

#include "core/agent/brain/fuzzybase.h"

/** \brief  Defuzz brain element

  The FuzzyDefuzz brain element... tbc

**/
class FuzzyDefuzz : public FuzzyBase
{
public:
    FuzzyDefuzz( quint32 id, Brain *brain, qreal defuzzVal);
    void calculate();

    /** \brief  returns the defuzzyfication value

      @returns the defuzzyfication value
      @sa Output

    **/
    qreal getDefuzzVal();

    /** \brief  returns the absolute defuzzyfication value

      the defuzz value is defined by a relative value whichis calculated by multiplying this value by the range of the connected min/max
      output value. If no Output is connected a value between 0 and 1 is returned

      @returns the defuzzyfication value
      @sa Output

    **/
    qreal getDefuzzValAbs();

    /** \brief  returns if this Defuzz is an else node
      @returns true, if this is an else node, false otherwise
    **/
    bool isElse();

    /** \brief  sets the defuzzyfication value

      the defuzz value is defined by a relative value which is calculated by multiplying this value by the range of the connected min/max
      output value

      @param val the relative defuzz value to the min/max output value range
      @sa Output

    **/
    void setDefuzzVal(qreal val);

    /** \brief  sets the defuzzyfication value

      the given value is internally changed to a relative defuzz value

      @param val the absolute defuzz value to the min/max output value range
      @sa FuzzyDefuzz::setDefuzzVal(qreal val)
      @sa Output

    **/
    void setDefuzzValAbs(qreal val);
    void setElse(bool isElse);

protected:
    qreal m_defuzzVal;
    bool m_isElse;
};

#endif // FUZZYDEFUZZ_H
