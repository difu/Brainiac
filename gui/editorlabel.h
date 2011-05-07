#ifndef EDITORLABEL_H
#define EDITORLABEL_H

#include <QLabel>
#include "core/brainiacglobals.h"

class EditorLabel : public QLabel
{
    Q_OBJECT
public:
    explicit EditorLabel(BrainiacGlobals::ItemType labelType);
protected:
    BrainiacGlobals::ItemType m_type;
    void mousePressEvent(QMouseEvent *ev);
signals:

public slots:

};

#endif // EDITORLABEL_H
