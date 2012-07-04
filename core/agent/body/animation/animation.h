#ifndef ANIMATION_H
#define ANIMATION_H

#include<QHash>

class AnimationCurve;

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

    Animation(const Animation& animation);
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
     * @brief returns, if this is a looped Animation
     *
     * @fn isLoopedAnimation
     * @return bool true, if this Animation is a loop animation
     */
    bool isLoopedAnimation() const { return m_isLoopedAnimation; }
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
    virtual ~Animation();
protected:
    QHash<QString, AnimationCurve*> m_curves; /**< all AnimationCurves, QString is the curve name, that correspondents to the agent´s SkeletonNode channel @sa SkeletonNode @sa Channel */
    QString m_name; /**< name of this Animation */
    bool m_isLoopedAnimation;
};

#endif // ANIMATION_H
