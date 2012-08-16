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


#ifndef SKELETONGEOMETRYNODE_H
#define SKELETONGEOMETRYNODE_H

#include "qglscenenode.h"

/**
 * @brief Represents the drawn geometry of a SkeletonNode (e.g. Sphere)
 * @todo (real) Silhouette drawing
 * @class SkeletonGeometryNode skeletongeometrynode.h "core/agent/body/skeletongeometrynode.h"
 */
class SkeletonGeometryNode : public QGLSceneNode
{
public:
    SkeletonGeometryNode();
    virtual void draw(QGLPainter *painter);
    void showSilhouette(bool show);
    void setColor(QColor col) {m_color=col;showSilhouette(m_showSilhouette);}

protected:
    virtual void drawGeometry(QGLPainter *painter);
    bool m_showSilhouette;
    QColor m_color;
};

#endif // SKELETONGEOMETRYNODE_H
