#ifndef TRANSITIONCURVE_H
#define TRANSITIONCURVE_H

#include<QList>
#include<QVector2D>

/**
 * @brief manages a transition curve
 *
 * @class TransitionCurve transitioncurve.h "core/agent/body/animation/transitioncurve.h"
 */
class LatchCurve
{
public:

/**
 * @brief
 *
 * @fn LatchCurve
 */
    LatchCurve();

 /**
 * @brief
 *
 * @fn LatchCurve
 * @param curve
 */
    LatchCurve(const LatchCurve &curve);
    /**
     * @brief
     *
     * @fn addLatch
     * @param time
     * @param duration
     */
    void addLatch(qreal time, qreal duration);

    /**
     * @brief
     *
     * @fn isLatch
     * @param time
     * @return bool
     */
    bool isLatch(qreal time) const;

    /**
     * @brief deletes a latch at given time
     *
     * @fn deleteLatch
     * @param time
     */
    void deleteLatch(qreal time);
    QList<QVector2D> latches() const;
protected:
    QList<QVector2D> m_latches; /**< all latches */
};

#endif // TRANSITIONCURVE_H
