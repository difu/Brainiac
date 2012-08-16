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


#ifndef LOOPEDITORSCENE_H
#define LOOPEDITORSCENE_H

#include <QGraphicsScene>
#include <QMutex>
#include <QHash>
#include <QList>

class EditorLineItem;
class ModifiableAnimation;
class AnimationCurves;


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
     * @brief sets the curves to display
     *
     * @fn setAnimationCurveNames
     * @param curveNames the name of the curves to display
     */
    void setAnimationCurveNames(QList <QString> curveNames);

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

    /**
     * @brief updates the curves
     *
     * @fn updateCurves
     */
    void updateCurves();
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
     * @brief maps the range of a curve to the height of the editor
     *
     * @fn mapCurveRangeToHeight
     * @param max
     * @param min
     * @param value
     * @return qreal
     */
    qreal mapCurveRangeToHeight(qreal max, qreal min, qreal value) const;

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
    QHash<QString, QList<QGraphicsItem *> > m_curveItems;
    QList<QString> m_curveItemNames;
    ModifiableAnimation *m_animation; /**< the Animation in this editor */
    QMutex m_animChangeMutex; /**< TODO */
    EditorLineItem *m_timeCurserItem; /**< TODO */
    EditorLineItem *m_startCurserItem; /**< TODO */
    EditorLineItem *m_endCurserItem; /**< TODO */
    EditorLineItem *m_crossFadeEndItem; /**< TODO */
    EditorLineItem *m_crossFadeStartItem; /**< TODO */
};

#endif // LOOPEDITORSCENE_H
