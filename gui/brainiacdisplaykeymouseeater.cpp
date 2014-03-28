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

#include "brainiacdisplaykeymouseeater.h"
#include <QEvent>
#include <QKeyEvent>
#include "core/brainiaclogger.h"

BrainiacDisplayKeyMouseEater::BrainiacDisplayKeyMouseEater(QObject *parent) :
    QObject(parent)
{
}

bool BrainiacDisplayKeyMouseEater::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        qCDebug(bGui) << __PRETTY_FUNCTION__ << "Key pressed " << keyEvent->key();
        emit keyPressed((Qt::Key)keyEvent->key());
        return false; // continue event processing
    } else if(event->type() == QEvent::MouseButtonPress) {
        qCDebug(bGui) << __PRETTY_FUNCTION__ << "mouse pressed";
        emit mouseButtonPressed();
        return false;
    } else if(event->type() == QEvent::MouseButtonRelease) {
        qCDebug(bGui) << __PRETTY_FUNCTION__ << "mouse released";
        return false;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
