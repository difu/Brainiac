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


#ifndef ANIMATIONCURVE_H
#define ANIMATIONCURVE_H

#include <QList>
#include <QVector2D>

/**
 * @brief Describes a curve of an Animation
 *
 * @class AnimationCurve animationcurve.h "core/agent/body/animation/animationcurve.h"
 */
class AnimationCurve
{
public:
/**
 * @brief constructs a new AnimationCurve
 *
 * @fn AnimationCurve
 */
    AnimationCurve();
/**
 * @brief constructs a new AnimationCurve as a exact copy of given AnimationCurve
 *
 * @fn AnimationCurve
 * @param curve
 */
    AnimationCurve(AnimationCurve *curve);
    /**
     * @brief adds a keyframe to this AnimationCurve
     *
     * @fn addKeyFrame
     * @param time the time of this keyframe
     * @param value the value of this keyframe
     */
    void addKeyFrame(qreal time, qreal value);

    /**
     * @brief adds a keyframe to this AnimationCurve
     *
     * @fn addKeyFrame
     * @param kf the keyframe, x is time, y is value

     */
    void addKeyFrame(QVector2D kf);

    AnimationCurve* clone();

    /**
     * @brief deletes all keyframes before time
     *
     * @fn deleteBeforeTime
     * @param time the time before all keyframes will be deleted
     */
    void deleteBeforeTime(qreal time);

    /**
     * @brief deletes all keyframes after time
     *
     * @fn deleteAfterTime
     * @param time the time after all keyframes will be deleted
     */
    void deleteAfterTime(qreal time);

    void generateIndexes() const;

    bool indexesDirty() const { return m_indexesDirty; }
    /**
     * @brief returns the length of this curve
     *
     * the length is the time of the last keyframe
     *
     * @returns qreal the length
     */
    qreal length() const { return m_keyFrames.last().x(); }

    /**
     * @brief get the maximal value of all keyframes
     *
     * if this curve does not have any keyframes 0 is returned
     * @fn getMaxValue
     * @return qreal the max value of all keyframes
     */
    qreal getMaxValue() const;

    /**
     * @brief get the min value of all keyframes
     *
     * if this curve does not have any keyframes 0 is returned
     * @fn getMinValue
     * @return qreal the min value of all keyframes
     */
    qreal getMinValue() const;

    /**
     * @brief returns the interpolated value for given time
     *
     * @fn getValue
     * @param time the time
     * @return qreal the interpolated value
     */
    qreal getValue(qreal time) const;

    /**
     * @brief returns a list of all key frames
     *
     * @fn keyFrames
     * @return QList<QVector2D>
     */
    QList<QVector2D> keyFrames() const {return m_keyFrames;}
    QList<QVector2D>& keyFrames()  {return m_keyFrames;}

    /**
     * @brief prints out all key frames
     *
     * Prints out information of all key frames. Useful for debugging
     * @fn dPrintKeyFrames
     * @param start
     * @param end
     */
    void dPrintKeyFrames(quint32 start, quint32 end) const;
    virtual ~AnimationCurve();
    /**
     * @brief the distance between indexes of keyframes
     */
    static const int IndexKeyFrameDistance;


protected:
    QList<QVector2D> m_keyFrames; /**< all keyframes */
    mutable bool m_indexesDirty; /**< if true, indexes should be newly generated */
    mutable QList<qreal> m_indexes; /**< Array that holds the indexes */

};

#endif // ANIMATIONCURVE_H
