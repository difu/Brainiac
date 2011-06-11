#ifndef EDITORITEM_H
#define EDITORITEM_H

#include <QGraphicsItem>
#include "core/brainiacglobals.h"

class EditorItemConnector;

/** \brief  Base class for items of editors

        All QGraphicsScene based items must subclass this

**/
class EditorItem : public QGraphicsItem
{
public:
    EditorItem(BrainiacGlobals::ItemType type, void *object, quint32 id);
    enum { Type = UserType + 15 };
    void addConnector(EditorItemConnector *connector);
    QRectF boundingRect() const;
    quint32 getId() { return m_id; }
    void * getObject() { return m_object; }
    BrainiacGlobals::ItemType  getType() {return m_type;}
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void removeConnector(EditorItemConnector *connector);
    void removeConnectors();
    QPainterPath shape() const;

    static const qint32 relxPos = -150;
    static const qint32 relyPos = -50;

    int type() const {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

protected:
    void *m_object;
    quint32 m_id;
    BrainiacGlobals::ItemType m_type;
    static const qint32 _height = 50;
    static const qint32 _width = 120;
    static const qint32 _raster = 10;
    qreal adjust;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    QImage m_symbolPic;
    QList<EditorItemConnector *> m_connectors;

};

#endif // EDITORITEM_H
