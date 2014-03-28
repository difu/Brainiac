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

#ifndef BRAINIACDISPLAYKEYMOUSEEATER_H
#define BRAINIACDISPLAYKEYMOUSEEATER_H

#include <QObject>

/**
 * @brief Filters out mouse button press/release and key press events and emits appropriate signals
 *
 * @class BrainiacDisplayKeyMouseEater brainiacdisplaykeymouseeater.h "gui/brainiacdisplaykeymouseeater.h"
 */
class BrainiacDisplayKeyMouseEater : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor
     *
     * @fn BrainiacDisplayKeyMouseEater
     * @param parent Parent object
     */
    explicit BrainiacDisplayKeyMouseEater(QObject *parent = 0);
protected:
    /**
     * @brief Filters events if this object has been installed as an event filter for the watched object
     *
     * @param obj the object
     * @param event the event to filter
     * @return bool true, if event is to be handled further
     */
    virtual bool eventFilter(QObject *obj, QEvent *event);
signals:
    /**
     * @brief emitted, if a key is pressed
     *
     * @fn keyPressed
     * @param key th pressed key
     */
    void keyPressed(Qt::Key key);

    /**
     * @brief emitted, if a mouse button is pressed
     *
     * @fn mouseButtonPressed
     */
    void mouseButtonPressed();

    /**
     * @brief emmited if a mouse button ios released
     *
     * @fn mouseButtonReleased
     */
    void mouseButtonReleased();
};

#endif // BRAINIACDISPLAYKEYMOUSEEATER_H
