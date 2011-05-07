#ifndef SCENEEDITORITEM_H
#define SCENEEDITORITEM_H

#include <QGraphicsItem>
#include "gui/editorlabel.h"
#include "core/brainiacglobals.h"

class SceneEditorItem : public QGraphicsItem
{
public:
    SceneEditorItem(BrainiacGlobals::ItemType type, void *object);
    QRectF boundingRect() const;
    void * getObject() { return m_object; }
    BrainiacGlobals::ItemType  getType() {return m_type;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

protected:
    void *m_object;
    BrainiacGlobals::ItemType m_type;
    static const qint32 relxPos = -150;
    static const qint32 relyPos = -50;
    static const qint32 _height = 50;
    static const qint32 _width = 120;
    static const qint32 _raster = 10;
    qreal adjust;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    QImage m_symbolPic;
};

#endif // SCENEEDITORITEM_H
