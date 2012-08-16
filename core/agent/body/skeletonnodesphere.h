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


#ifndef SKELETONNODESPHERE_H
#define SKELETONNODESPHERE_H
#include "skeletonnode.h"
#include "qgraphicsscale3d.h"
class Channel;

class SkeletonNodeSphere : public SkeletonNode
{
    Q_OBJECT
public:
    SkeletonNodeSphere(quint32 id, const QString &name, Body *body);
    /**
     * @brief sets the radius of this SkeletonNodeSphere
     *
     * Sets the initial radius of this sphere. Internally it sets all dimensions of the geometry´s scale to radius
     *
     * @param radius the radius of this sphere
     */
    void setRadius(qreal radius);
    /**
     * @brief returns the radius of this SkeletonNodeSphere
     *
     * Returns the radius. Internally only the x dimension is returned as per definitiony the scale of x=y=z
     *
     * @return qreal the radius
     */
    qreal getRadius() const { return m_segScale->scale().x(); }
protected:
    void createChannels();
    Channel *m_radius;
protected slots:
    void channelRadiusChanged(qreal value);
};

#endif // SKELETONNODESPHERE_H
