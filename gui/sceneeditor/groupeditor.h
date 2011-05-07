#ifndef GROUPEDITOR_H
#define GROUPEDITOR_H

#include <QWidget>
#include "gui/itemeditorwidgetsbase.h"

class Group;

namespace Ui {
    class GroupEditor;
}

class GroupEditor : public QWidget, public ItemEditorWidgetsBase
{
    Q_OBJECT

public:
    explicit GroupEditor(Scene *scene, QWidget *parent = 0);
    void setGroup( Group* group );
    ~GroupEditor();

protected:
    void changeEvent(QEvent *e);
    Group *m_group;

protected slots:
    void groupNameChanged();

private:
    Ui::GroupEditor *ui;
};

#endif // GROUPEDITOR_H
