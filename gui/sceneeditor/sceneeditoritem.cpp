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

