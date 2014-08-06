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



#ifndef BVHMANAGER_H
#define BVHMANAGER_H

#include <QObject>
#include "core/brainiacglobals.h"

class AgentManager;

/**
 * @brief BVH Motion Manager
 *
 * provides funtionality to open, save, modify BVH motion data
 *
 * @class BvhManager bvhmanager.h "core/agent/body/animation/bvhmanager.h"
 */
class BvhManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Options for BVH output
     *
     * @enum BvhOptions
     */
    enum BvhOptions {
        Default=0, /**< Simple write out bvh skeleton */
        PositionAsBone=1 /**< Add additional bone that contains the transformation of the agent */
    };

    /**
     * @brief constructor
     *
     * @fn BvhManager
     * @param parent
     */
    explicit BvhManager(AgentManager *parent);

    /**
     * @brief creates a bvh conform string containing the rotation translation order
     *
     * @fn getBvhRotTransFromBrainiacRotTrans
     * @param list list of Brainiac order
     * @return QString bvh string of order
     */
    static QString getBvhRotTransFromBrainiacRotTrans(const QList<BrainiacGlobals::RotTrans> &list);

    /**
     * @brief returns the list of channels according to bvh skeleton
     *
     * @fn getBVHMotionChannelList
     * @return const QString
     */
    const QList<QString>& getBVHMotionChannelList() const;

    quint32 getBvhRootId() const;
    /**
     * @brief returns the BVH representation of the skeleton of the agent
     *
     * the motion data is not included!
     *
     * @fn getBVHSkeleton
     * @return const QString the BVH data
     */
    const QString& getBVHSkeleton() const;

    /**
     * @brief returns the options
     *
     * @fn getOptions
     * @return BvhOptions
     */
    BvhOptions getOptions() const { return m_options; }

    /**
     * @brief marks channel list dirty
     *
     * @fn setChannelListDirty
     */
    void setChannelListDirty();

    /**
     * @brief sets the options
     * setChannelListDirty() and setSkeletonDirty() will be called implicitly
     * @fn setOptions
     * @param options
     */
    void setOptions(BvhOptions options);

    /**
     * @brief marks skeleton dirty.
     * next get will issue recalculation of internal structures
     *
     * @fn setDirty
     */
    void setSkeletonDirty();

private:
    mutable QString m_bvhSkeleton; /**< holds the bvh representation of the skeleton */
    mutable bool m_bvhSkeletonDirty; /**< true, if the bvh representation has to be recalculated */
    mutable bool m_bvhSegmentChannelListDirty; /**< true, if the bvh representation has to be recalculated */
    mutable QList<QString> m_bvhChannelList; /** traveresed list of motionchannels of bvh skeleton */
    mutable QString m_bvhMotionData; //!< contains actual bvh motion data @sa getBvhMotionData()
    quint32 m_bvhRootId;
    BvhOptions m_options;
signals:

public slots:

};

#endif // BVHMANAGER_H
