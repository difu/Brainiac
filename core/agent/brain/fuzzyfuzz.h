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


#ifndef FUZZYFUZZ_H
#define FUZZYFUZZ_H
#include <QEasingCurve>

#include "core/agent/brain/fuzzybase.h"

/** \brief  Fuzzy fuzz node

**/
class FuzzyFuzz : public FuzzyBase
{
public:
    /** \brief  the fuzzyfication mode / the membership
    **/
    enum Mode {
               DIRAC, //!< fuzzy return value is true, if input val==p1
               ACTIVATE, //!< the returnvalue is 0 if value is below p1, has a slope if between p1 and p2 and 1 if value>p2
               DEACTIVATE, //!< inverse result of Mode::ACTIVATE
               TRIANGLE, //!< return value depends on triangle defined by p1, p2, p3
               TRAPEZOID //!< return value depends on trapezoid p1, p2, p3, p4
              };
    enum InterpolationMode {LINEAR, SINE, QUAD };
    FuzzyFuzz( quint32 id, Brain *brain, QString name, Mode mode, InterpolationMode iMode);
    void calculate();

    /** \brief  triggers calculation but emitting the resultChanged() Signal is selectable

      Sometimes it is useful not to emit the resultChanged() Signal, e.g. if a sound and node has to determine the best input combinations
        @sa FuzzyBase::resultChanged()
    **/
    void calculate(bool emitChange);

    /** \brief  returns the fuzzy value
        if this fuzz is connected to a node, it returns the value with respect to the parent´s bounds
        if not, it sets the bounds to 0 and 1
            @returns the fuzzyfication value
    **/
    qreal getFuzzOut( qreal val ) const;
    /** \brief  returns the absolute fuzzy value with respect to given bounds
            @param min lower bound
            @param max higher bound
            @returns the fuzzyfication value
    **/
    qreal getFuzzOut( qreal min, qreal max, qreal val ) const;

    /** \brief  returns the Mode of this fuzz rule
        @returns the fuzzyfication membership/mode
        @sa Mode
    **/
    Mode getMode() const;

    /** \brief  returns the relative value of the first point defining the membership curve
        @returns the relative value
    **/
    qreal getP1() const;
    /** \brief  returns the relative value of the second point defining the membership curve
        @returns the relative value
    **/
    qreal getP2() const;
    /** \brief  returns the relative value of the third point defining the membership curve
        @returns the relative value
    **/
    qreal getP3() const;
    /** \brief  returns the relative value of the 4th point defining the membership curve
        @returns the relative value
    **/
    qreal getP4() const;

    /** \brief  returns the absolute value of the first point defining the membership curve
      If the node doesn´t have an incoming node the relative value is returned instead
        @returns the absolute value
    **/
    qreal getP1Abs() const;
    /** \brief  returns the absolute value of the 2nd point defining the membership curve
      If the node doesn´t have an incoming node the relative value is returned instead
        @returns the absolute value
    **/
    qreal getP2Abs() const;
    /** \brief  returns the absolute value of the 3rd point defining the membership curve
      If the node doesn´t have an incoming node the relative value is returned instead
        @returns the absolute value
    **/
    qreal getP3Abs() const;
    /** \brief  returns the absolute value of the 4th point defining the membership curve
      If the node doesn´t have an incoming node the relative value is returned instead
        @returns the absolute value
    **/
    qreal getP4Abs() const;

    /** \brief  returns this fuzz´s parent´s minimum

            @returns the minimum value of its parent, 0 if it has no parent. This is the default min value of any fuzz
    **/
    qreal getParentMin() const;
    /** \brief  returns this fuzz´s parent´s max

            @returns the maximum value of its parent, 1 if it has no parent. This is the default max value of any fuzz
    **/
    qreal getParentMax() const;

    /** \brief  returns the interpolatin mode

            @returns the interpolation mode
            @sa InterpolationMode
    **/
    InterpolationMode getInterpolationMode() const;

    /** \brief  sets the interpolatin mode

            @param mode the interpolation mode
            @sa InterpolationMode
    **/
    void setInterpolationMode(InterpolationMode mode);

    /** \brief  sets the fuzzification membership funtion
      @sa Mode
    **/
    void setMode(Mode mode);

    /** \brief  sets the 1st point of the member function
      Depending on the mode (the membership function) this point is set or corrected to fit into its bounds\n
      The bounds depend on the fuzz´s mode
            @param p1 the relativ ordinate of 1st point of the membership function
            @param check if true, the parameter is corrected to its bounds
            @returns the (possible corrected) value pf the 1st member function point
            @sa Mode
    **/
    qreal setP1(qreal p1, bool check=false);

    /** \brief  sets the 2nd point of the member function
      Depending on the mode (the membership function) this point is set or corrected to fit into its bounds\n
      The bounds depend on the fuzz´s mode
            @param p2 the relativ ordinate of 2nd point of the membership function
            @param check if true, the parameter is corrected to its bounds
            @returns the (possible corrected) value pf the 2nd member function point
    **/
    qreal setP2(qreal p2, bool check=false);

    /** \brief  sets the 3rd point of the member function
      Depending on the mode (the membership function) this point is set or corrected to fit into its bounds\n
      The bounds depend on the fuzz´s mode
            @param p3 the relativ ordinate of 3rd point of the membership function
            @param check if true, the parameter is corrected to its bounds
            @returns the (possible corrected) value pf the 3rd member function point
    **/
    qreal setP3(qreal p3, bool check=false);

    /** \brief  sets the 4th point of the member function
      Depending on the mode (the membership function) this point is set or corrected to fit into its bounds\n
      The bounds depend on the fuzz´s mode
            @param p4 the relativ ordinate of 4th point of the membership function
            @param check if true, the parameter is corrected to its bounds
            @returns the (possible corrected) value pf the 4th member function point
    **/
    qreal setP4(qreal p4, bool check=false);
    static const qreal MinDistBetweenPoints=0.000001f; //!< the minimum distance between two membership poinzs as they must never overlap

protected:
    Mode m_mode; //!< Fuzzyfication mode
    InterpolationMode m_interpolationMode; //!< interpolation mode
    qreal m_p1, //!< 1st point of the member function
    m_p2,//!< 2nd point of the member function
    m_p3,//!< 3rd point of the member function
    m_p4;//!< 4th point of the member function
    QEasingCurve m_easing; //!< Interpolation curve
};

#endif // FUZZYFUZZ_H
