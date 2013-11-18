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


#ifndef ANIMATION_H
#define ANIMATION_H

#include<QHash>
#include<QReadWriteLock>
#include "core/brainiacglobals.h"


class AnimationCurve;
class LatchCurve;

/**
 * @brief Describes an Animation and provides access
 *
 * An Animation consists of one or more AnimationCurves, which describe the rotation or translation of the skeleton´s segements
 * @sa AnimationCurve
 * @class Animation animation.h "core/agent/body/animation/animation.h"
 */
class Animation
{
public:
/**
 * @brief constructs a new Animation without any AnimationCurves
 *
 * @fn Animation
 */
    Animation();

    Animation(Animation *animation);
/**
 * @brief
 *
 * @sa AnimationCurve
 * @fn Animation
 * @param curves the AnimationCurves to add to this new Animation
 * @param name the name of this Animation
 */
    Animation(QHash<QString, AnimationCurve*> curves, QString name);

    /**
     * @brief add a curve with given name
     *
     * @param curveName the name of the curve to be added
     */
    void addCurve(const QString &curveName);

    /**
     * @brief add a curve with given name
     *
     * @param curveName the name of the curve to be added
      *@param curve the curve to be added
     */
    void addCurve(const QString &curveName, AnimationCurve curve);

    /**
     * @brief add a latchcurve with given name
     *
     * @param clatchName the name of the latch curve to be added
     */
    void addLatchCurve(const QString &latchName);

    void addLatch(const QString &latchName, qreal start, qreal length);
    void addLatch(const QString &latchName, QVector2D latch);

    BrainiacGlobals::AnimationType animationType() const { return m_animType; }

    /**
     * @brief calculate the length of this Animation
     *
     * The length is calculated as the max keyframe time of the longest AnimationCurve
     * @fn calculateLength
     */
    void calculateLength() const;

    void copyFromAnimation(Animation *animation);

    /**
     * @brief deletes the curve with name curve
     *
     * @param curveName the name of the curve to be deleted
     */
    void deleteCurve(const QString &curveName);

    /**
     * @brief deletes all curves
     *
     * all segment curves, latch curves and the transition curve will be deleted
     */
    void deleteCurves();

    /**
     * @brief returns the length of this Animation
     *
     * the length of an Animation is always the maximum time value of all keyframes of all AnimatoinCurve s
     * @fn getLength
     * @param calculateNew true if the length should re calculated, false if a cached length should be returned
     * @return qreal the length of this Animation
     */
    qreal getLength(bool calculateNew=false) const;


    virtual qreal getValue(const QString &curve, qreal time) const;

    /**
     * @brief returns, if this is a looped Animation
     *
     * @fn isLoopedAnimation
     * @return bool true, if this Animation is a loop animation
     */
    bool isLoopedAnimation() const { return m_isLoopedAnimation; }

    /**
     * @brief returns if this Animation is retriggerable from the brain
     *
     * @fn isRetriggerable
     * @return bool true, if this Animation is retriggerable
     */
    bool isRetriggerable() const { return m_isRetriggerable; }

    /**
     * @brief loads a brainiac animation file
     *
     * @param fileName
     * @return Animation
     */
    static Animation* loadAnimation(QString fileName);
    /**
     * @brief returns the filename
     *
     * the filename
     * @fn fileName
     * @return QString
     */
    QString fileName() const {return m_fileName; }

    /**
     * @brief adds a new keyframe into the AnimationCurve curve
     *
     * @fn addKeyFrame
     * @param curveName the name of the AnimationCurve
     * @param time
     * @param value
     */
    void addKeyFrame(QString &curveName, qreal time, qreal value);

    AnimationCurve* getTransitionCurve() const;

    /**
     * @brief returns, if this Animation has AgentCurves
     *
     *
     * @fn hasAgentCurves
     * @return bool true, if this Animation has AgentCurves, false, if not
     */
    bool hasAgentCurves() const;
    /**
     * @brief the name of this Animation
     *
     * @fn name
     * @return QString the name
     */
    QString name() const { return m_name;}

    /**
     * @brief returns a modifiable reference to this Animation
     *
     * @fn curves
     * @return QHash<QString, AnimationCurve *> the reference to all the Animation´s AnimationCurves
     */
    QHash<QString, AnimationCurve*>& curves() {return m_curves;}
    QHash<QString, AnimationCurve*> curves() const {return m_curves;}
    QHash<QString, LatchCurve*> latches() const {return m_latchCurves;}
    /**
     * @brief returns a list of all AnimationCurve names
     *
     * @fn curveNames
     * @return QList<QString>
     */
    QList<QString> curveNames() const;

    bool saveAnimation();

    /**
     * @brief saves the animation with given absolute filename
     *
     * the filename also becomes the default filename
     * @sa saveAnimation()
     * @fn saveAnimation
     * @param fileName
     * @return bool
     */
    bool saveAnimation(QString &fileName);

    void setAnimationType( BrainiacGlobals::AnimationType type) { m_animType=type; }

    /**
     * @brief sets the filename of this Animation
     *
     * @fn setFileName
     * @param fileName
     */
    void setFileName(QString relativeFileName) {m_fileName=relativeFileName;}

    /**
     * @brief set if this Animation is to be looped
     *
     * @fn setIsLoopedAnimation
     * @param isLooped true, if this Animation should be looped
     */
    void setIsLoopedAnimation( bool isLooped ) { m_isLoopedAnimation=isLooped; }

    void setIsRetriggerable( bool isRetriggerable ) { m_isRetriggerable=isRetriggerable; }

    /**
     * @brief sets the name of this Animation
     *
     * @fn setName
     * @param name
     */
    virtual void setName(QString name) { m_name=name; }
    virtual ~Animation();
protected:
    mutable qreal m_length;
    QHash<QString, AnimationCurve*> m_curves; /**< all AnimationCurves, QString is the curve name, that correspondents to the agent´s BodySegement channel @sa BodySegment @sa Channel */
    QString m_name; /**< name of this Animation */
    QString m_fileName; /**< filename relative to scenefile */
    bool m_isLoopedAnimation;
    bool m_isRetriggerable;
    mutable QReadWriteLock m_rwLock; /**< Lock to prevent reading from curves when the curves are changed */
    BrainiacGlobals::AnimationType m_animType; /**< the type of this animation */
    QHash<QString, LatchCurve*> m_latchCurves; /**< all latchCurves, QString is the curve name */
    AnimationCurve *m_transitionCurve; /**< the transition curve */
};

#endif // ANIMATION_H
