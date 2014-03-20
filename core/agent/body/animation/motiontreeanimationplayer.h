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

#ifndef MOTIONTREEANIMATIONPLAYER_H
#define MOTIONTREEANIMATIONPLAYER_H

#include "core/agent/body/animation/animationplayer.h"
#include <QObject>

class Channel;
class MotionTree;

/**
 * @brief
 *
 * @class MotionTreeAnimationPlayer motiontreeanimationplayer.h "core/agent/body/animation/motiontreeanimationplayer.h"
 */
class MotionTreeAnimationPlayer : public AnimationPlayer
{
    Q_OBJECT
public:
    /**
     * @brief Constructor creates a new MotionTreeAnimationPlayer
     *
     * A MotionTreeAnimationPlayer reads out the MotionTree and decides what Actions and their corresponding Animations to play
     *
     * @fn MotionTreeAnimationPlayer
     * @param body the Body of this MotionTreeAnimationPlayer
     * @param tree the MotionTree of this MotionTreeAnimationPlayer
     *
     */
    MotionTreeAnimationPlayer(Body *body, MotionTree *tree);

    /**
     * @brief applies the Animation(s)
     *
     * @fn apply
     */
    void apply();

    /**
     * @brief resets the animationplayer to its initial state
     *
     * @fn reset
     */
    virtual void reset();

protected:
    MotionTree *m_motionTree; /**< the MotionTree */
    Channel *m_phaseChannel; /**< phase Channel (Input) */
    Channel *m_latchChannel; /**< latch Channel (Input) */
};

#endif // MOTIONTREEANIMATIONPLAYER_H
