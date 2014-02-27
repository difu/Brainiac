#ifndef MOTIONTREEEDITORITEMCONNECTOR_H
#define MOTIONTREEEDITORITEMCONNECTOR_H
#include "gui/editoritemconnector.h"

class MotionTreeEditorItemConnector : public EditorItemConnector
{
public:
    MotionTreeEditorItemConnector(EditorItem *startItem, EditorItem *endItem);
};

#endif // MOTIONTREEEDITORITEMCONNECTOR_H
