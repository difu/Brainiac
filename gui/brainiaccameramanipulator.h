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


#ifndef BRAINIACCAMERAMANIPULATOR_H
#define BRAINIACCAMERAMANIPULATOR_H

#ifdef BRAINIAC_SUPPRESS_THIRD_PARTY_WARNINGS
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
#include <osgGA/TrackballManipulator>
#ifdef BRAINIAC_SUPPRESS_THIRD_PARTY_WARNINGS
    #pragma clang diagnostic pop
#endif

/**
 * @brief Manipulates the camera.
 * Provides functionality to move, orbit, pan, zoom a camera
 * @class BrainiacCameraManipulator brainiaccameramanipulator.h "gui/brainiaccameramanipulator.h"
 */
class BrainiacCameraManipulator : public osgGA::TrackballManipulator
{
public:
    /**
     * @brief constructs a new BrainiacCameraManipulator
     *
     * @fn BrainiacCameraManipulator
     */
    BrainiacCameraManipulator();
protected:
    virtual void centerMousePointer( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
    virtual bool handleKeyDown(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);
    virtual bool handleKeyUp(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);
    virtual bool handleMousePush(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);
    virtual bool handleMouseWheel(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);
    virtual bool performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy);
private:
    bool m_centerKeysPressed; /**< true, if one of the keys is pressed that centers the view */

};

#endif // BRAINIACCAMERAMANIPULATOR_H
