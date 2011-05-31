#ifndef EDITORITEM_H
#define EDITORITEM_H

#include <QGraphicsItem>
#include "core/brainiacglobals.h"

/** \brief  Base class for items of editors

        All QGraphicsScene based items must subclass this

**/
class EditorItem : public QGraphicsItem
{
public:
    EditorItem(BrainiacGlobals::ItemType type, void *object, quint32 id);
    QRectF boundingRect() const;
    quint32 getId() { return m_id; }
    void * getObject() { return m_object; }
    BrainiacGlobals::ItemType  getType() {return m_type;}
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

protected:
    void *m_object;
    quint32 m_id;
    BrainiacGlobals::ItemType m_type;
    static const qint32 relxPos = -150;
    static const qint32 relyPos = -50;
    static const qint32 _height = 50;
    static const qint32 _width = 120;
    static const qint32 _raster = 10;
    qreal adjust;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    QImage m_symbolPic;

};

#endif // EDITORITEM_H
