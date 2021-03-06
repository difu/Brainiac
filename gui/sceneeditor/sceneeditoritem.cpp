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


#include "sceneeditoritem.h"
#include <QLinearGradient>
#include <QPainter>
#include <QPalette>
#include <QDebug>
#include "core/group/group.h"
#include "core/agent/agentmanager.h"

SceneEditorItem::SceneEditorItem(BrainiacGlobals::ItemType type, void *object)  : EditorItem(type, object,0)
{

}

