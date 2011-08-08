#ifndef BRAINEDITORITEN_H
#define BRAINEDITORITEN_H

#include "gui/editoritem.h"
#include "gui/editorlabel.h"
#include "core/brainiacglobals.h"

#include <QImage>

class BrainEditorItem : public EditorItem
{
public:
    BrainEditorItem(BrainiacGlobals::ItemType type, void *object,quint32 id);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    QImage m_featurePic;
};

#endif // BRAINEDITORITEN_H
