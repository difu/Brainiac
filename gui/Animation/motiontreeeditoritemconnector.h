#ifndef MOTIONTREEEDITORITEMCONNECTOR_H
#define MOTIONTREEEDITORITEMCONNECTOR_H
#include "gui/editoritemconnector.h"
#include "gui/editoritem.h"

class MotionTreeEditorItemConnector : public EditorItemConnector
{
public:
    MotionTreeEditorItemConnector(EditorItem *startItem, EditorItem *endItem);
    void setProgress( qreal t ) { m_progress=t; }
    virtual ~MotionTreeEditorItemConnector();

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = 0);
    qreal m_progress;
};

#endif // MOTIONTREEEDITORITEMCONNECTOR_H
