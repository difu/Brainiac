#ifndef LOOPEDITORSCENE_H
#define LOOPEDITORSCENE_H

#include <QGraphicsScene>
#include <QMutex>

class EditorLineItem;
class ModifiableAnimation;


/**
 * @brief Allows editing start-, endpoint and crossfade of an animation
 *
 * @todo skimming?
 * @class LoopEditorScene loopeditorscene.h "gui/Animation/loopeditorscene.h"
 */
class LoopEditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
/**
 * @brief Constructs a new LoopEditorScene
 *
 * @fn LoopEditorScene
 */
    LoopEditorScene();
    /**
     * @brief sets the to be edited Animation
     *
     * @fn setAnimation
     * @param animation the animation
     */
    void setAnimation(ModifiableAnimation *animation);
    /**
     * @brief sets the time cursor to a given time
     *
     * the location is matched to the scene
     * @fn setTimeCursor
     * @param time the time in ms
     */
    void setTimeCursor(qreal time);
    /**
     * @brief updates the LoopEditorScene
     *
     * The whole content is updated to actual values
     * @fn update
     */
    void update();

public slots:
    /**
     * @brief
     *
     * @fn updateTime
     * @param time
     */
    void updateTime(qreal time);

protected:
    /**
     * @brief maps the widgets width to a time value
     *
     * @fn mapWidthToTime
     * @param width the width from which the time is calculated
     * @return qreal the time in ms
     */
    qreal mapWidthToTime(qreal width) const;
    /**
     * @brief maps a time to the widgets width (x position)
     *
     * @fn mapTimeToWidth
     * @param time time in ms
     * @return qreal the width (x position
     */
    qreal mapTimeToWidth(qreal time) const;
    ModifiableAnimation *m_animation; /**< the Animation in this editor */
    QMutex m_animChangeMutex; /**< TODO */
    EditorLineItem *m_timeCurserItem; /**< TODO */
    EditorLineItem *m_startCurserItem; /**< TODO */
    EditorLineItem *m_endCurserItem; /**< TODO */
    EditorLineItem *m_crossFadeEndItem; /**< TODO */
    EditorLineItem *m_crossFadeStartItem; /**< TODO */
};

#endif // LOOPEDITORSCENE_H
