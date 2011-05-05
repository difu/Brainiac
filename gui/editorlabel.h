#ifndef EDITORLABEL_H
#define EDITORLABEL_H

#include <QLabel>

class EditorLabel : public QLabel
{
    Q_OBJECT
public:
    enum EditorLabelType {AND, OR, FUZZ, DEFUZZ, OUTPUT, INPUT, NOISE, TIMER, SPHERE, CUBE};
    explicit EditorLabel(EditorLabelType labelType);
protected:
    EditorLabelType m_type;
    void mousePressEvent(QMouseEvent *ev);
signals:

public slots:

};

#endif // EDITORLABEL_H
