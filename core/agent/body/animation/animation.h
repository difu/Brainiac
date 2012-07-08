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
     * @brief calculate the length of this Animation
     *
     * The length is calculated as the max keyframe time of the longest AnimationCurve
     * @fn calculateLength
     */
    void calculateLength();

    /**
     * @brief returns the length of this Animation
     *
     * the length of an Animation is always the maximum time value of all keyframes of all AnimatoinCurve s
     * @fn getLength
     * @param calculateNew true if the length should re calculated, false if a cached length should be returned
     * @return qreal the length of this Animation
     */
    qreal getLength(bool calculateNew=false);
    /**
     * @brief returns, if this is a looped Animation
     *
     * @fn isLoopedAnimation
     * @return bool true, if this Animation is a loop animation
     */

    virtual qreal getValue(const QString &curve, qreal time) const;

    bool isLoopedAnimation() const { return m_isLoopedAnimation; }

    /**
     * @brief returns the filename
     *
     * the filename relative to the scenefile location
     * @fn fileName
     * @return QString
     */
    QString relativeFileName() const {return m_fileName; }

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

    /**
     * @brief sets the filename of this Animation
     *
     * the filename should be relative to the scene file location
     * the absolute path is calculated outside
     * @fn setFileName
     * @param fileName
     */
    void setRelativeFileName(QString relativeFileName) {m_fileName=relativeFileName;}
    virtual ~Animation();
protected:
    qreal m_length;
    QHash<QString, AnimationCurve*> m_curves; /**< all AnimationCurves, QString is the curve name, that correspondents to the agent´s SkeletonNode channel @sa SkeletonNode @sa Channel */
    QString m_name; /**< name of this Animation */
    QString m_fileName; /**< filename relative to scenefile */
    bool m_isLoopedAnimation;
};

#endif // ANIMATION_H