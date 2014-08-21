// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2014  Dirk Fuchs dirkfux@googlemail.com

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

/*
* StandardManipulator code Copyright (C) 2010 PCJohn (Jan Peciva)
* while some pieces of code were taken from OSG.
* Thanks to company Cadwork (www.cadwork.ch) and
* Brno University of Technology (www.fit.vutbr.cz) for open-sourcing this work.
*/
/*
* OrbitManipulator code Copyright (C) 2010 PCJohn (Jan Peciva)
* while some pieces of code were taken from OSG.
* Thanks to company Cadwork (www.cadwork.ch) and
* Brno University of Technology (www.fit.vutbr.cz) for open-sourcing this work.
*/

#include "brainiaccameramanipulator.h"
#include <QGuiApplication>
#include <QScreen>

using namespace osgGA;

BrainiacCameraManipulator::BrainiacCameraManipulator() : osgGA::TrackballManipulator()
{
    setAnimationTime(0);
}

void BrainiacCameraManipulator::centerMousePointer(const GUIEventAdapter &ea, GUIActionAdapter &us)
{
    qreal ratio=((QGuiApplication*)QCoreApplication::instance())->primaryScreen()->physicalDotsPerInch() /
                ((QGuiApplication*)QCoreApplication::instance())->primaryScreen()->logicalDotsPerInch();

    _mouseCenterX = ((ea.getXmin() + ea.getXmax()) / 2.0f) / ratio;
    _mouseCenterY = ((ea.getYmin() + ea.getYmax()) / 2.0f) * ratio;
    us.requestWarpPointer( _mouseCenterX, _mouseCenterY );
}

bool BrainiacCameraManipulator::handleMouseWheel(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
    osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();

    // handle centering
    //if( _flags & SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT )
    {

        if( ((sm == GUIEventAdapter::SCROLL_DOWN )) ||
            ((sm == GUIEventAdapter::SCROLL_UP   )) )
        {

            if( getAnimationTime() <= 0. )
            {
                // center by mouse intersection (no animation)
                setCenterByMousePointerIntersection( ea, us );
            }
            else
            {
                // start new animation only if there is no animation in progress
                if( !isAnimating() )
                    startAnimationByMousePointerIntersection( ea, us );

            }

        }
    }

    switch( sm )
    {
        // mouse scroll up event
        case GUIEventAdapter::SCROLL_UP:
        {
            // perform zoom
            zoomModel( _wheelZoomFactor, true );
            us.requestRedraw();
            us.requestContinuousUpdate( isAnimating() || _thrown );
            return true;
        }

        // mouse scroll down event
        case GUIEventAdapter::SCROLL_DOWN:
        {
            // perform zoom
            zoomModel( -_wheelZoomFactor, true );
            us.requestRedraw();
            us.requestContinuousUpdate( isAnimating() || _thrown );
            return true;
        }

        // unhandled mouse scrolling motion
        default:
            return false;
   }
}

bool BrainiacCameraManipulator::performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    // zoom model
    Q_UNUSED(dy);
    zoomModel( dx * getThrowScale( eventTimeDelta ), true );
    return true;
}
