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


#ifndef MODIFIABLEANIMATION_H
#define MODIFIABLEANIMATION_H
#include "core/agent/body/animation/animation.h"
#include "core/brainiacglobals.h"

class Body;

/**
 * @brief ModifiableAnimation provides functionality to manipulate and optimize an Animation
 *
 * @todo x,z rotation for agent transformation, rotation order
 * @todo xyz translation for agent transformation
 * @class ModifiableAnimation modifiableanimation.h "core/agent/body/animation/modifiableanimation.h"
 */
class ModifiableAnimation : public Animation
{
public:
/**
 * @brief
 *
 * @fn ModifiableAnimation
 * @param animation
 */
    ModifiableAnimation(Animation *animation, Body *body);
    virtual ~ModifiableAnimation();

    /**
     * @brief Bakes the applied changes to the curve
     *
     */
    void bake();

    /**
     * @brief creates AgentCurves
     * per default a latch curve is created with a latch at 1/2 second before the end of this Animation
     *
     * the curves will be only permanently added if bake() is called afterwards
     * if AgentCurves already exist, nothing will be changed and false will be returned
     * @sa resetAgentCurves() if you want to delete agentcurves that are not baked
     * @sa bake() if you want to add the AgentCurves permanently
     * @fn createAgentCurves
     * @returns bool true, if AgentCurves were created
     */
    bool createAgentCurves();
    /**
     * @brief returns if root rx curve should be cross faded
     *
     * @fn crossFadeRx
     * @return bool
     */
    bool crossFadeRx() const {return m_crossFadeRx; }
    bool crossFadeRy() const {return m_crossFadeRy; }
    bool crossFadeRz() const {return m_crossFadeRz; }
    bool crossFadeTx() const {return m_crossFadeTx; }
    bool crossFadeTy() const {return m_crossFadeTy; }
    bool crossFadeTz() const {return m_crossFadeTz; }

    /**
     * @brief returns the value of a certain time of a curve
     *
     * If time is not on a keyframe, the value is calculated. Returns a crossfaded value, if crossfading!=0
     * @fn getValue
     * @param curve the curve
     * @param time the time
     * @return qreal the interpolated value
     */
    virtual qreal getValue(const QString &curve, qreal time) const;

    /**
     * @brief returns the root bone translation
     *
     * Use this, if the agent has not any root curves yet
     * useful for camera following
     * @fn getRootBoneTranslation
     * @param time
     * @return QVector3D the translation of the root bone
     */
    QVector3D getRootBoneTranslation(qreal time) const;

    /**
     * @brief resets the AgentCurves
     *
     * Resets the agent´s root curves and deletes agent curves in case the preview was not accepted
     * @fn resetAgentCurves
     */
    void resetAgentCurves();

    /**
     * @brief sets the type of the Animation
     *
     * this sets the proper suggested root curves for crossfading
     * any changes afterward direct via setCrossFadeRootCurves() does not change the type!
     * @fn setAnimationType
     * @param type
     */
    void setAnimationType(BrainiacGlobals::AnimationType type);
    /**
     * @brief
     *
     * @fn setStartTime
     * @param startTime
     */
    void setStartTime(qreal startTime);
    /**
     * @brief
     *
     * @fn setEndTime
     * @param endTime
     */
    void setEndTime(qreal endTime);

    /**
     * @brief sets the crossfade in milisecs
     *
     * @fn setCrossFade
     * @param crossFade
     */
    void setCrossFade(qreal crossFade);
    /**
     * @brief sets which root curves should be cross faded
     *
     * This does not change the animation type!
     * @fn setCrossFadeRootCurves
     * @param rx
     * @param ry
     * @param rz
     * @param tx
     * @param ty
     * @param tz
     */
    void setCrossFadeRootCurves(bool rx, bool ry, bool rz, bool tx, bool ty, bool tz);

    /**
     * @brief sets the name of this ModifiableAnimation
     *
     * This function alters also the name of the original Animation
     * @fn setName
     * @param name
     */
    virtual void setName(QString name) {Animation::setName(name);m_origAnimation->setName(name);}

    /**
     * @brief
     * @bug Y Rotation currently only works fine when y angle>0 && <180. Even at the borders some minor erros occur, works best at 90°. Obviously bug/error/... in matrix decompositions
     * @param yAxisRot
     */
    void setTansformRotation(qreal yAxisRot);

    qreal getCrossFade() const {return m_crossFadeTime;}
    qreal getStartTime() const {return m_startTime;}
    qreal getEndTime() const {return m_endTime;}
    bool isRootBoneCurve( const QString &curve ) const;

    static const qreal minDistTime=0.001f; /**< minimum time distance between start and end in ms */
protected:

    /**
     * @brief creates an agent curve from a root curve
     *
     * Also the root curve gets modified
     *
     * @fn createAgentCurve
     * @param rootCurve pointer to the root curve
     * @param agentCurve pointer to the agent curve
     */
    static void createAgentCurve(AnimationCurve *rootCurve, AnimationCurve *agentCurve);

    /**
     * @brief returns a cross faded value
     *
     * @fn getCrossfadedValue
     * @param curve name of curve to crossfade
     * @param time the time for this crossfade
     * @return qreal the crossfaded value
     * @bug first and lastframe have the same value, if cf!=0 the first frame should not be the last frame. Workaround in code....
     */
    qreal getCrossfadedValue(const QString &curve, qreal time) const;

    qreal m_startTime; /**< start time of Animation in ms */
    qreal m_endTime; /**< end time of Animation in ms */
    qreal m_crossFadeTime; /**< duration of crossfade */
    bool m_crossFadeRx;
    bool m_crossFadeRy;
    bool m_crossFadeRz;
    bool m_crossFadeTx;
    bool m_crossFadeTy;
    bool m_crossFadeTz;
    Body *m_body;
    Animation *m_origAnimation;
};

#endif // MODIFIABLEANIMATION_H
