#ifndef EDITORLINEITEM_H
#define EDITORLINEITEM_H

#include <QGraphicsLineItem>

/**
 * @brief A line item in an editor
 *
 * @class EditorLineItem editorlineitem.h "gui/Animation/editorlineitem.h"
 */
class EditorLineItem : public QGraphicsLineItem
{
public:
/**
 * @brief
 *
 * @fn EditorLineItem
 * @param scene
 */
    EditorLineItem(QGraphicsScene *scene);
};

#endif // EDITORLINEITEM_H
