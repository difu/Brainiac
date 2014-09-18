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


#include "fuzzyor.h"

FuzzyOr::FuzzyOr( quint32 id, Brain *brain, Mode mode) : FuzzyBase(FuzzyBase::OR, brain, id, 0, 1),m_mode(mode)
{
}

void FuzzyOr::calculate()
{
    if( m_parents.count()>0) {
        qreal tmpResult=m_minValue;
        foreach(Parent parent, m_parents) {
            qreal parentResult=parent.parent->getResult(parent.inverted);
            switch(m_mode) {
            case MAX:
                if(parentResult>tmpResult)
                    tmpResult=parentResult;
                break;
            case SUM:
                tmpResult+=parentResult;
            }
        }
        setResult(tmpResult);
    } else {
        setResult(0);
    }
}

FuzzyOr::Mode FuzzyOr::getMode()
{
    return m_mode;
}

void FuzzyOr::setMode(Mode mode)
{
    m_mode=mode;
}
