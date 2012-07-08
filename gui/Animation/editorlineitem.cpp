#include "editorlineitem.h"
#include <QGraphicsScene>
#include <QPen>

#include <QDebug>

EditorLineItem::EditorLineItem(QGraphicsScene *scene):QGraphicsLineItem(0,scene)
{
    setLine(0,0,0,200);
}
