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


#ifndef ANIMATIONPLAYER_H
#define ANIMATIONPLAYER_H
#include <QObject>
#include <QHash>
#include <QStateMachine>


class Animation;
class Body;
class Simulation;

/** @brief  AnimationPlayer plays animations activated from an agents brain or by its MotionTree
 * @sa MotionTree
**/
class AnimationPlayer: public QObject
{
    Q_OBJECT

public:
    /** @brief  Constructor
     * @param Body body the body this AnimationPlayer belongs to
    **/
    AnimationPlayer( Body *body);

    /** @brief  applies triggered animations
     * This plays all animations and blends triggered directly from the brain or/and the MotionTree
    **/
    void apply();

    /** @brief  Applies an Animation
     * @param animation the Animation to be applied
     * @param val a value between 0.0 and 1.0 that is the progress of the animation
    **/
    void apply2(const Animation &animation, qreal val);

    /** @brief  Applies an Animation
     * @param animation the Animation to be applied
     * @param time the time of the animation
    **/
    void apply(const Animation &animation, qreal time);

    Animation* getCurrentAnimation() { return m_currentAnimation; }

    /**
     * @brief resets everything
     *
     * all animations and states are cleared
     */
    virtual void reset();
protected:
    Animation *m_currentAnimation; /**< Pointer to current Animation */
    bool m_currentAnimationStartedForFirstTime; /**< true, if the Animation is running for the 1st time during this loop */
    qreal m_currentAnimationStartTime; /**< time when the current animation was started in Simulation time */
    Animation *m_nextAnimation; /**< the next animation that is to be started or already has started */
    qreal m_nextAnimationStartTime; /**< time when the next animation was started in Simulation time */
    Simulation *m_simulation;
    Body *m_body;

signals:
    void animCanTransit();
    void animCannotTransit();
    void animIsInTransition();
    void aninHasTransitioned();
};

#endif // ANIMATIONPLAYER_H
