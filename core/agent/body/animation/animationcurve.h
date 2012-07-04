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
    AnimationCurve(const AnimationCurve& curve);
    /**
     * @brief adds a keyframe to this AnimationCurve
     *
     * @fn addKeyFrame
     * @param time the time of this keyframe
     * @param value the value of this keyframe
     */
    void addKeyFrame(qreal time, qreal value);

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

protected:
    QList<QVector2D> m_keyFrames; /**< all keyframes */
};

#endif // ANIMATIONCURVE_H
